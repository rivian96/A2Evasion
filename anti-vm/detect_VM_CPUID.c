#include <windows.h>
#include <stdio.h>
#include <intrin.h>
#include <string.h>

void check_hypervisor() {
    int cpuInfo[4] = {0};
    char vendor[13] = {0};  // 12 chars + null terminator

    // 1. CPUID leaf for hypervisor vendor ID
    // This fills the array with data from registers EAX, EBX, ECX, EDX
    __cpuid(cpuInfo, 0x40000000);

    // 2. Vendor string is stored in EBX, ECX, EDX (indices 1, 2, 3)
    memcpy(vendor + 0, &cpuInfo[1], 4); // EBX
    memcpy(vendor + 4, &cpuInfo[2], 4); // ECX
    memcpy(vendor + 8, &cpuInfo[3], 4); // EDX

    const char* hypervisor = NULL;

    // 3. Compare against known Vendor Signatures
    if (strcmp(vendor, "VMwareVMware") == 0)
        hypervisor = "VMware";
    else if (strcmp(vendor, "VBoxVBoxVBox") == 0)
        hypervisor = "VirtualBox";
    else if (strcmp(vendor, "Microsoft Hv") == 0)
        hypervisor = "Hyper-V";
    else if (strcmp(vendor, "KVMKVMKVM") == 0)
        hypervisor = "KVM";
    else if (strcmp(vendor, "XenVMMXenVMM") == 0)
        hypervisor = "Xen";
    else if (strcmp(vendor, "prl hyperv") == 0)
        hypervisor = "Parallels";

    // 4. Act on the result
    if (hypervisor) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Hypervisor detected: %s", hypervisor);
        MessageBoxA(NULL, msg, "Anti-VM Check", MB_OK | MB_ICONWARNING);
    } else {
        // No hypervisor detected (or check bypassed) -> launch calculator
        WinExec("calc.exe", SW_SHOW);
    }
}

int main() {
    check_hypervisor();
    return 0;
}.exe