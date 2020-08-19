[#ftl]
[#list configs as dt]
[#assign data = dt]
[#assign peripheralParams = dt.peripheralParams]
[#assign peripheralGPIOParams = dt.peripheralGPIOParams]
[#assign peripheralDMAParams = dt.peripheralDMAParams]
[#assign peripheralNVICParams = dt.peripheralNVICParams]
[#assign usedIPs = dt.usedIPs]
/******************************************************************************
 * File Name   : MX_Device.h
 * Date        : ${date}
 * Description : STM32Cube MX parameter definitions
 * Note        : This file is generated by STM32CubeMX (DO NOT EDIT!)
 ******************************************************************************/

#ifndef __MX_DEVICE_H
#define __MX_DEVICE_H


/*---------------------------- Clock Configuration ---------------------------*/

[#list [
    "LSI_VALUE",
    "LSE_VALUE",
    "HSI_VALUE",
    "HSE_VALUE",
    "EXTERNAL_CLOCK_VALUE",
    "PLLCLKFreq_Value",
    "PLLQCLKFreq_Value",
    "SYSCLKFreq_VALUE",
    "HCLKFreq_Value",
    "FCLKCortexFreq_Value",
    "CortexFreq_Value",
    "AHBFreq_Value",
    "APB1Freq_Value",
    "APB2Freq_Value",
    "APB1TimFreq_Value",
    "APB2TimFreq_Value",
    "48MHZClocksFreq_Value",
    "EthernetFreq_Value",
    "I2SClocksFreq_Value",
    "RTCFreq_Value",
    "WatchDogFreq_Value",
    "MCO1PinFreq_Value",
    "MCO2PinFreq_Value"
  ] as key]
[#if peripheralParams.get("RCC").get(key)??]
#define MX_${key?right_pad(36)} ${peripheralParams.get("RCC").get(key)}
[/#if]
[/#list]

[#list usedIPs as ip]
[#if peripheralParams.get(ip)??]
/*-------------------------------- ${ip?right_pad(10)} --------------------------------*/

#define ${("MX_" + ip)?right_pad(39)} 1

[#if ip?starts_with("USART")]
[#list peripheralParams.get(ip).entrySet() as paramEntry]
[#if paramEntry.key?starts_with("VirtualMode")]
#define ${("MX_" + ip + "_VM")?right_pad(39)} ${paramEntry.value}
[/#if]
[/#list]
[/#if]
[#if ip?starts_with("UART")]
[#list peripheralParams.get(ip).entrySet() as paramEntry]
[#if paramEntry.key?starts_with("Mode")]
[#if paramEntry.value?starts_with("IRDA")]
#define ${("MX_" + ip + "_VM")?right_pad(39)} VM_IRDA
[#else]
#define ${("MX_" + ip + "_VM")?right_pad(39)} VM_ASYNC
[/#if]
[/#if]
[/#list]
[/#if]
[#if ip?starts_with("USB_OTG")]
[#list peripheralParams.get(ip).entrySet() as paramEntry]
[#if paramEntry.key?starts_with("VirtualMode")]
[#if paramEntry.value?starts_with("Device")]
#define ${("MX_" + ip + "_DEVICE")?right_pad(39)} 1
[#elseif paramEntry.value?starts_with("Host")]
#define ${("MX_" + ip + "_HOST")?right_pad(39)} 1
[#else]
#define ${("MX_" + ip + "_DEVICE")?right_pad(39)} 1
#define ${("MX_" + ip + "_HOST")?right_pad(39)} 1
[/#if]
[/#if]
[/#list]
[/#if]

[#if peripheralGPIOParams.get(ip)??]
/* GPIO Configuration */

[#list peripheralGPIOParams.get(ip).entrySet() as gpioParamEntry]
[#assign PinName=gpioParamEntry.value.get("Pin")]
/* Pin ${PinName} */
[#assign PinName=PinName?replace("-","_")]
[#assign PinName=PinName?replace(" ","_")]
[#assign PinName=PinName?replace("/","_")]
[#list gpioParamEntry.value.entrySet() as gpioParam]
[#if gpioParam.value?length > 0]
[#assign ParamValue=gpioParam.value]
[#assign ParamValue=ParamValue?replace("-","_")]
[#assign ParamValue=ParamValue?replace(" ","_")]
[#assign ParamValue=ParamValue?replace("/","_")]
[#if ip == "GPIO"]
[#if     gpioParam.key == "GPIO_Label"]
#define ${("MX_" + ParamValue)?right_pad(39)} ${PinName}
[#elseif gpioParam.key != "Port"]
#define ${("MX_" + PinName + "_" + gpioParam.key)?right_pad(39)} ${ParamValue}
[/#if]
[#else]
[#if     gpioParam.key == "GPIO_Label"]
#define ${("MX_" + ParamValue)?right_pad(39)} ${gpioParamEntry.key}
[#elseif gpioParam.key != "Port"]
#define ${("MX_" + gpioParamEntry.key + "_" + gpioParam.key)?right_pad(39)} ${ParamValue}
[/#if]
[/#if]
[/#if]
[/#list]

[/#list]
[/#if]
[#if peripheralDMAParams.get(ip)?? && peripheralDMAParams.get(ip).entrySet()?size > 0]
/* DMA Configuration */

[#list peripheralDMAParams.get(ip).entrySet() as dmaParamEntry]
/* DMA ${dmaParamEntry.key} */
[#list dmaParamEntry.value.entrySet() as dmaParam]
#define ${("MX_" + dmaParamEntry.key + "_DMA_" + dmaParam.key)?right_pad(39)} ${dmaParam.value}
[/#list]

[/#list]
[/#if]
[#if peripheralNVICParams.get(ip)?? && peripheralNVICParams.get(ip).entrySet()?size > 0]
/* NVIC Configuration */

[#list peripheralNVICParams.get(ip).entrySet() as nvicParamEntry]
/* NVIC ${nvicParamEntry.key} */
[#list nvicParamEntry.value.entrySet() as nvicParam]
#define ${("MX_" + nvicParamEntry.key + "_" + nvicParam.key)?right_pad(39)} ${nvicParam.value}
[/#list]

[/#list]
[/#if]
[/#if]
[/#list]

#endif  /* __MX_DEVICE_H */

[/#list]
