console.log("[*] Safe Mode: Hunting for Hypervisor CPUID checks...");

var cpuid_pattern = "0F A2"; // The CPUID instruction
var magic_bytes = "00 00 00 40"; // The signature of 0x40000000

function safe_patch() {
    var module = Process.enumerateModules()[0];

    Memory.scan(module.base, module.size, cpuid_pattern, {
        onMatch: function(address, size) {
            
            // 1. We found a CPUID. Now let's verify if it's the specific one we hate.
            // We read the 64 bytes coming BEFORE this instruction.
            var search_window = 64;
            var context_bytes = address.sub(search_window).readByteArray(search_window);
            
            // Convert to hex string for easy searching
            var context_hex = toHexString(context_bytes);

            // 2. Check if "00 00 00 40" is in those previous bytes
            if (context_hex.includes("00 00 00 40")) {
                
                console.log("[!] THREAT CONFIRMED at " + address);
                console.log("    Found '0x40000000' setup nearby. This is the Hypervisor Check.");

                // 3. Apply the Patch
                try {
                    Memory.protect(address, 2, 'rwx');
                    address.writeByteArray([0x31, 0xC0]); // XOR EAX, EAX
                    console.log("    [+] PATCHED successfully.");
                } catch (e) {
                    console.log("    [-] Patch failed: " + e);
                }

            } else {
                // This is likely a legitimate CPU feature check (like checking for AVX)
                console.log("[*] Ignored safe CPUID at " + address + " (No 0x40000000 signature found)");
            }
        },
        onComplete: function() {
            console.log("[*] Scan complete.");
        }
    });
}

// Helper function to convert raw bytes to a clean Hex string
function toHexString(byteArray) {
  return Array.from(new Uint8Array(byteArray))
    .map(function(byte) {
      return ('0' + (byte & 0xFF).toString(16)).slice(-2);
    }).join(' ');
}

safe_patch();