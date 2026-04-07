# A2Evasion
A2 Evasion is a security automation tool designed to neutralize sophisticated anti-virtualization (Anti-VM) techniques in real-time. By leveraging Dynamic Binary Instrumentation (DBI), the tool intercepts CPU instructions like CPUID and neutralizes them to mask the analysis environment, forcing malware to execute its true behavior.


# Installation

Before we install this tool make sure you have frida-tools installed on your system
```
git clone https://github.com/rivian96/A2Evasion.git
cd A2Evasion/anti-vm
```

