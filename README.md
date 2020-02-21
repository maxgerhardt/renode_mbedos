# Renode + mbedos

## Building 

* download [PlatformIO](https://docs.platformio.org/en/latest/installation.html#python-package-manager)
* clone this repo
* `pio run` 
* 
```
$ pio run 
Processing disco_f407vg (platform: ststm32; board: disco_f407vg; framework: mbed)
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Verbose mode can be enabled via `-v, --verbose` option
CONFIGURATION: https://docs.platformio.org/page/boards/ststm32/disco_f407vg.html
PLATFORM: ST STM32 6.0.0 > ST STM32F4DISCOVERY
HARDWARE: STM32F407VGT6 168MHz, 128KB RAM, 1MB Flash
DEBUG: Current (stlink) On-board (stlink) External (blackmagic, jlink)
PACKAGES: 
 - framework-mbed 5.51401.200110 (5.14.1) 
 - toolchain-gccarmnoneeabi 1.70201.0 (7.2.1)
Collecting mbed sources...
LDF: Library Dependency Finder -> http://bit.ly/configure-pio-ldf
LDF Modes: Finder ~ chain, Compatibility ~ soft
Found 4 compatible libraries
Scanning dependencies...
No dependencies
Building in release mode
Compiling .pio/build/disco_f407vg/FrameworkMbed/drivers/source/AnalogIn.o
[...]
Linking .pio/build/disco_f407vg/firmware.elf
Checking size .pio/build/disco_f407vg/firmware.elf
Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"
RAM:   [          ]   3.1% (used 4048 bytes from 131072 bytes)
Flash: [=         ]   5.2% (used 54660 bytes from 1048576 bytes)
Building .pio/build/disco_f407vg/firmware.bin
======================== [SUCCESS] Took 38.66 seconds =======================
```
* generates `.pio/build/disco_f407vg/firmware.elf` 

## Expected results 

UART2 output ("Hello world") and "Test" every second. LED state toggling every second.

## Used renode script 

Adapted from `stm32f4_discovery.resc`

```
:name: STM32F4 Discovery
:description: This script runs Contiki on STM32F4 Discovery.

using sysbus
$name?="STM32F4_Discovery"
mach create $name
machine LoadPlatformDescription @platforms/boards/stm32f4_discovery-kit.repl

cpu PerformanceInMips 125

: ADAPT PATH HERE!
$bin?=@/home/max/f4_test_renode/prebuilt_firmware.elf

: adapted to UART2
showAnalyzer sysbus.uart2

### Set random board UNIQUE ID ###

python "import _random"
python "rand = _random.Random()"

$id1 = `python "print rand.getrandbits(32)"`
$id2 = `python "print rand.getrandbits(32)"`
$id3 = `python "print rand.getrandbits(32)"`
macro reset
"""
    sysbus LoadELF $bin

    sysbus WriteDoubleWord 0x1FFF7A10 $id1
    sysbus WriteDoubleWord 0x1FFF7A14 $id2
    sysbus WriteDoubleWord 0x1FFF7A18 $id3
"""

runMacro $reset

```

Start in the renode console with `s @/<path to script>`.

## Execution results

In the UART2 analyzer, nothing shows up. 

In the renode console, messagegs are spammed regarding reading TIM5->CNT. 

```
16:22:48.0917 [INFO] Loaded monitor commands from: /home/max/Downloads/renode/./scripts/monitor.py
16:22:51.1325 [INFO] Including script: /home/max/Downloads/renode/scripts/single-node/stm32f4_discovery_mbedos.resc
16:22:51.1513 [INFO] System bus created.
16:22:51.8518 [DEBUG] Segment size automatically calculated to value 16MiB
16:22:51.8526 [DEBUG] Segment size automatically calculated to value 64KiB
16:22:51.8526 [DEBUG] Segment size automatically calculated to value 128KiB
16:22:52.0647 [DEBUG] Segment size automatically calculated to value 64KiB
16:22:52.5890 [INFO] sysbus: Loaded SVD: /tmp/renode-15140/afb8bbc3-1299-4ce8-853b-f22b896aa170.tmp. Name: STM32F40x. Description: STM32F40x.
16:22:52.9131 [DEBUG] sysbus: Loading ELF /home/max/f4_test_renode/prebuilt_firmware.elf.
16:22:52.9285 [INFO] sysbus: Loading segment of 51892 bytes length at 0x8000000.
16:22:52.9417 [DEBUG] sysbus: Segment loaded.
16:22:52.9419 [INFO] sysbus: Loading segment of 2768 bytes length at 0x800CAB4.
16:22:52.9421 [DEBUG] sysbus: Segment loaded.
16:22:52.9422 [INFO] sysbus: Loading segment of 1280 bytes length at 0x800D588.
16:22:52.9423 [DEBUG] sysbus: Segment loaded.
16:22:52.9424 [INFO] sysbus: Loading segment of 256 bytes length at 0x20000188.
16:22:52.9435 [DEBUG] sysbus: Segment loaded.
16:22:53.0524 [INFO] cpu: Guessing VectorTableOffset value to be 0x8000000.
16:22:53.0558 [INFO] cpu: Setting initial values: PC = 0x80024F5, SP = 0x20020000.
16:22:53.0611 [INFO] STM32F4_Discovery: Machine started.
16:22:53.1767 [WARNING] sysbus: [cpu: 0x80023A8] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:22:53.1793 [DEBUG] sysbus: Write value 0xA020083 to RCC:CR (0x40023800).
16:22:53.1795 [DEBUG] sysbus: Write value 0x0 to RCC:CFGR (0x40023808).
16:22:53.1796 [WARNING] sysbus: [cpu: 0x80023A8] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:22:53.1813 [DEBUG] sysbus: Write value 0xA020083 to RCC:CR (0x40023800).
16:22:53.1815 [DEBUG] sysbus: Write value 0x24003010 to RCC:PLLCFGR (0x40023804).
16:22:53.1815 [WARNING] sysbus: [cpu: 0x80023A8] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:22:53.1835 [DEBUG] sysbus: Write value 0xA020083 to RCC:CR (0x40023800).
16:22:53.1835 [DEBUG] sysbus: Write value 0x0 to RCC:CIR (0x4002380C).
16:22:53.1931 [WARNING] sysbus: [cpu: 0x8003234] (tag: 'RCC_CFGR') ReadDoubleWord from non existing peripheral at 0x40023808, returning 0x19940A.
16:22:53.1933 [WARNING] sysbus: [cpu: 0x8003264] (tag: 'RCC_PLLCFGR') ReadDoubleWord from non existing peripheral at 0x40023804, returning 0x0.
16:22:53.1933 [WARNING] sysbus: [cpu: 0x8003264] (tag: 'RCC_PLLCFGR') ReadDoubleWord from non existing peripheral at 0x40023804, returning 0x0.
16:22:53.1933 [WARNING] sysbus: [cpu: 0x8003264] (tag: 'RCC_PLLCFGR') ReadDoubleWord from non existing peripheral at 0x40023804, returning 0x0.
16:22:53.1933 [WARNING] sysbus: [cpu: 0x8003264] (tag: 'RCC_PLLCFGR') ReadDoubleWord from non existing peripheral at 0x40023804, returning 0x0.
16:22:53.1934 [WARNING] sysbus: [cpu: 0x800324A] (tag: 'RCC_CFGR') ReadDoubleWord from non existing peripheral at 0x40023808, returning 0x19940A.
16:22:53.1941 [DEBUG] sysbus: Read value 0x0 from FLASH:ACR (0x40023C00).
16:22:53.1942 [DEBUG] sysbus: Write value 0x200 to FLASH:ACR (0x40023C00).
16:22:53.1943 [DEBUG] sysbus: Read value 0x0 from FLASH:ACR (0x40023C00).
16:22:53.1943 [DEBUG] sysbus: Write value 0x400 to FLASH:ACR (0x40023C00).
16:22:53.1943 [DEBUG] sysbus: Read value 0x0 from FLASH:ACR (0x40023C00).
16:22:53.1944 [DEBUG] sysbus: Write value 0x100 to FLASH:ACR (0x40023C00).
16:22:53.1963 [WARNING] sysbus: [cpu: 0x8002A78] (tag: 'RCC_CFGR') ReadDoubleWord from non existing peripheral at 0x40023808, returning 0x19940A.
16:22:53.1963 [WARNING] sysbus: [cpu: 0x8002A78] (tag: 'RCC_CFGR') ReadDoubleWord from non existing peripheral at 0x40023808, returning 0x19940A.
16:22:53.1963 [WARNING] sysbus: [cpu: 0x8002A78] (tag: 'RCC_CFGR') ReadDoubleWord from non existing peripheral at 0x40023808, returning 0x19940A.
16:22:53.1964 [WARNING] sysbus: [cpu: 0x8002A78] (tag: 'RCC_CFGR') ReadDoubleWord from non existing peripheral at 0x40023808, returning 0x19940A.
16:22:53.1964 [DEBUG] sysbus: Read value 0x0 from FLASH:ACR (0x40023C00).
16:22:53.1965 [WARNING] sysbus: [cpu: 0x8002A38] (tag: 'RCC_CFGR') ReadDoubleWord from non existing peripheral at 0x40023808, returning 0x19940A.
16:22:53.1966 [DEBUG] sysbus: Read value 0x0 from RCC:APB1ENR (0x40023840).
16:22:53.1966 [DEBUG] sysbus: Write value 0x8 to RCC:APB1ENR (0x40023840).
16:22:53.1967 [DEBUG] sysbus: Read value 0x0 from RCC:APB1ENR (0x40023840).
16:22:53.1967 [DEBUG] sysbus: Read value 0x0 from RCC:APB1RSTR (0x40023820).
16:22:53.1967 [DEBUG] sysbus: Write value 0x8 to RCC:APB1RSTR (0x40023820).
16:22:53.1968 [DEBUG] sysbus: Read value 0x0 from RCC:APB1RSTR (0x40023820).
16:22:53.1968 [DEBUG] sysbus: Write value 0x0 to RCC:APB1RSTR (0x40023820).
16:22:53.1977 [DEBUG] sysbus: Read value 0x0 from TIM5:CR1 (0x40000C00).
16:22:53.2020 [DEBUG] sysbus: Write value 0x0 to TIM5:CR1 (0x40000C00).
16:22:53.2020 [DEBUG] sysbus: Write value 0xFFFFFFFF to TIM5:ARR (0x40000C2C).
16:22:53.2021 [DEBUG] sysbus: Write value 0xFFFFFFFF to TIM5:PSC (0x40000C28).
16:22:53.2022 [DEBUG] sysbus: Write value 0x1 to TIM5:EGR (0x40000C14).
16:22:53.2030 [DEBUG] sysbus: Read value 0x0 from TIM5:CCER (0x40000C20).
16:22:53.2031 [DEBUG] sysbus: Write value 0x0 to TIM5:CCER (0x40000C20).
16:22:53.2031 [DEBUG] sysbus: Read value 0x0 from TIM5:CCER (0x40000C20).
16:22:53.2032 [DEBUG] sysbus: Write value 0x1 to TIM5:CCER (0x40000C20).
16:22:53.2033 [DEBUG] sysbus: Read value 0x0 from TIM5:CR1 (0x40000C00).
16:22:53.2034 [DEBUG] sysbus: Write value 0x1 to TIM5:CR1 (0x40000C00).
16:22:53.2035 [DEBUG] sysbus: Read value 0x0 from TIM5:DIER (0x40000C0C).
16:22:53.2035 [DEBUG] sysbus: Write value 0x0 to TIM5:DIER (0x40000C0C).
16:22:53.2061 [DEBUG] sysbus: Read value 0x0 from RCC:APB1ENR (0x40023840).
16:22:53.2062 [DEBUG] sysbus: Write value 0x10000000 to RCC:APB1ENR (0x40023840).
16:22:53.2062 [DEBUG] sysbus: Read value 0x0 from RCC:APB1ENR (0x40023840).
16:22:53.2063 [DEBUG] sysbus: Read value 0x0 from PWR:CR (0x40007000).
16:22:53.2064 [DEBUG] sysbus: Write value 0x0 to PWR:CR (0x40007000).
16:22:53.2064 [DEBUG] sysbus: Read value 0x0 from PWR:CR (0x40007000).
16:22:53.2066 [WARNING] sysbus: [cpu: 0x80025E0] (tag: 'RCC_CFGR') ReadDoubleWord from non existing peripheral at 0x40023808, returning 0x19940A.
16:22:53.2067 [WARNING] sysbus: [cpu: 0x80025EC] (tag: 'RCC_CFGR') ReadDoubleWord from non existing peripheral at 0x40023808, returning 0x19940A.
16:22:53.2068 [WARNING] sysbus: [cpu: 0x80025F6] (tag: 'RCC_PLLCFGR') ReadDoubleWord from non existing peripheral at 0x40023804, returning 0x0.
16:22:53.2069 [WARNING] sysbus: [cpu: 0x8002616] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:22:53.2070 [DEBUG] sysbus: Write value 0xA030083 to RCC:CR (0x40023800).
16:22:53.2071 [DEBUG] sysbus: Read value 0x0 from TIM5:CNT (0x40000C24).
16:22:53.2072 [WARNING] sysbus: [cpu: 0x8002622] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:22:53.2075 [WARNING] sysbus: [cpu: 0x800280E] (tag: 'RCC_CFGR') ReadDoubleWord from non existing peripheral at 0x40023808, returning 0x19940A.
16:22:53.2079 [DEBUG] sysbus: Read value 0x0 from RCC:APB1ENR (0x40023840).
16:22:53.2080 [DEBUG] sysbus: Write value 0x10000000 to RCC:APB1ENR (0x40023840).
16:22:53.2080 [DEBUG] sysbus: Read value 0x0 from RCC:APB1ENR (0x40023840).
16:22:53.2080 [DEBUG] sysbus: Read value 0x0 from PWR:CR (0x40007000).
16:22:53.2081 [DEBUG] sysbus: Write value 0x0 to PWR:CR (0x40007000).
16:22:53.2081 [DEBUG] sysbus: Read value 0x0 from PWR:CR (0x40007000).
16:22:53.2081 [WARNING] sysbus: [cpu: 0x80025A8] (tag: 'RCC_CFGR') ReadDoubleWord from non existing peripheral at 0x40023808, returning 0x19940A.
16:22:53.2081 [WARNING] sysbus: [cpu: 0x80025A8] (tag: 'RCC_CFGR') ReadDoubleWord from non existing peripheral at 0x40023808, returning 0x19940A.
16:22:53.2081 [WARNING] sysbus: [cpu: 0x80025A8] (tag: 'RCC_PLLCFGR') ReadDoubleWord from non existing peripheral at 0x40023804, returning 0x0.
16:22:53.2083 [WARNING] sysbus: [cpu: 0x800264A] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:22:53.2083 [DEBUG] sysbus: Write value 0xA020083 to RCC:CR (0x40023800).
16:22:53.2083 [WARNING] sysbus: [cpu: 0x800264A] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:22:53.2084 [DEBUG] sysbus: Write value 0xA020083 to RCC:CR (0x40023800).
16:22:53.2084 [DEBUG] sysbus: Read value 0x0 from TIM5:CNT (0x40000C24).
16:22:53.2085 [WARNING] sysbus: [cpu: 0x8002662] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:22:53.2086 [DEBUG] sysbus: Read value 0x0 from TIM5:CNT (0x40000C24).
16:22:53.2087 [WARNING] sysbus: [cpu: 0x8002664] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:22:53.2087 [DEBUG] sysbus: Read value 0x0 from TIM5:CNT (0x40000C24).
16:22:53.2087 [WARNING] sysbus: [cpu: 0x800266E] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:22:53.2087 [DEBUG] sysbus: Read value 0x0 from TIM5:CNT (0x40000C24).
16:22:53.2088 [WARNING] sysbus: [cpu: 0x800266E] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:22:53.2088 [DEBUG] sysbus: Read value 0x0 from TIM5:CNT (0x40000C24).
16:22:53.2088 [WARNING] sysbus: [cpu: 0x800266E] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:22:53.2088 [DEBUG] sysbus: Read value 0x0 from TIM5:CNT (0x40000C24).
16:22:53.2088 [WARNING] sysbus: [cpu: 0x800266E] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:22:53.2088 [DEBUG] sysbus: Read value 0x0 from TIM5:CNT (0x40000C24).
16:22:53.2089 [WARNING] sysbus: [cpu: 0x800266E] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:22:53.2089 [DEBUG] sysbus: Read value 0x0 from TIM5:CNT (0x40000C24).
16:22:53.2089 [WARNING] sysbus: [cpu: 0x800266E] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:22:53.2089 [DEBUG] sysbus: Read value 0x0 from TIM5:CNT (0x40000C24).
16:22:53.2089 [WARNING] sysbus: [cpu: 0x800266E] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:22:53.2089 [DEBUG] sysbus: Read value 0x0 from TIM5:CNT (0x40000C24).
16:22:53.2090 [WARNING] sysbus: [cpu: 0x800266E] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:22:53.2090 [DEBUG] sysbus: Read value 0x0 from TIM5:CNT (0x40000C24).
16:22:53.2090 [WARNING] sysbus: [cpu: 0x800266E] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:22:53.2090 [DEBUG] sysbus: Read value 0x0 from TIM5:CNT (0x40000C24).
16:22:53.2090 [WARNING] sysbus: [cpu: 0x800266E] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:22:53.2090 [DEBUG] sysbus: Read value 0x0 from TIM5:CNT (0x40000C24).
16:22:53.2091 [WARNING] sysbus: [cpu: 0x800266E] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
[... ad infinitum ...]
``` 

## Activating RTOS

See last line in `platformio.ini` and `prebuilt_firmware_rtos.elf`. This causes renode to idle at some point with still not UART2 output. 

```
16:36:51.7481 [INFO] Loaded monitor commands from: /home/max/Downloads/renode/./scripts/monitor.py
16:36:55.0670 [INFO] Including script: /home/max/Downloads/renode/scripts/single-node/stm32f4_discovery_mbedos.resc
16:36:55.0964 [INFO] System bus created.
16:36:55.8590 [DEBUG] Segment size automatically calculated to value 16MiB
16:36:55.8598 [DEBUG] Segment size automatically calculated to value 64KiB
16:36:55.8599 [DEBUG] Segment size automatically calculated to value 128KiB
16:36:56.0666 [DEBUG] Segment size automatically calculated to value 64KiB
16:36:56.5543 [INFO] sysbus: Loaded SVD: /tmp/renode-20116/5aa29524-622a-43ea-9653-7532953becaa.tmp. Name: STM32F40x. Description: STM32F40x.
16:36:56.8959 [DEBUG] sysbus: Loading ELF /home/max/Downloads/renode/FWs/firmware_rtos.elf.
16:36:56.9220 [INFO] sysbus: Loading segment of 57712 bytes length at 0x8000000.
16:36:56.9385 [DEBUG] sysbus: Segment loaded.
16:36:56.9387 [INFO] sysbus: Loading segment of 10056 bytes length at 0x800E170.
16:36:56.9388 [DEBUG] sysbus: Segment loaded.
16:36:56.9389 [INFO] sysbus: Loading segment of 256 bytes length at 0x20000188.
16:36:56.9405 [DEBUG] sysbus: Segment loaded.
16:36:57.0958 [INFO] cpu: Guessing VectorTableOffset value to be 0x8000000.
16:36:57.0988 [INFO] cpu: Setting initial values: PC = 0x8003AC1, SP = 0x20020000.
16:36:57.1225 [INFO] STM32F4_Discovery: Machine started.
16:36:57.2579 [WARNING] sysbus: [cpu: 0x8003974] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:36:57.2599 [DEBUG] sysbus: Write value 0xA020083 to RCC:CR (0x40023800).
16:36:57.2600 [DEBUG] sysbus: Write value 0x0 to RCC:CFGR (0x40023808).
16:36:57.2600 [WARNING] sysbus: [cpu: 0x8003974] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:36:57.2600 [DEBUG] sysbus: Write value 0xA020083 to RCC:CR (0x40023800).
16:36:57.2601 [DEBUG] sysbus: Write value 0x24003010 to RCC:PLLCFGR (0x40023804).
16:36:57.2602 [WARNING] sysbus: [cpu: 0x8003974] (tag: 'RCC_CR') ReadDoubleWord from non existing peripheral at 0x40023800, returning 0xA020083.
16:36:57.2602 [DEBUG] sysbus: Write value 0xA020083 to RCC:CR (0x40023800).
16:36:57.2602 [DEBUG] sysbus: Write value 0x0 to RCC:CIR (0x4002380C).
16:36:57.2635 [WARNING] nvic: Unhandled read from offset 0xD90.
16:36:57.2649 [WARNING] sysbus: [cpu: 0x8001332] ReadDoubleWord from non existing peripheral at 0x3C.
16:36:57.2650 [WARNING] sysbus: [cpu: 0x8001332] ReadDoubleWord from non existing peripheral at 0x34.
16:36:57.2650 [WARNING] sysbus: [cpu: 0x8001332] ReadDoubleWord from non existing peripheral at 0x30.
[nothing]
``` 
