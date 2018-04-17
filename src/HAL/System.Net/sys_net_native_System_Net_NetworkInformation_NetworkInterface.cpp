//
// Copyright (c) 2018 The nanoFramework project contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
// See LICENSE file in the project root for full license information.
//

#include "sys_net_native.h"

void Network_PostEvent( unsigned int eventType, unsigned int flags )
{
    PostManagedEvent( EVENT_NETWORK, eventType, flags, 0 );
}

HRESULT Library_sys_net_native_System_Net_NetworkInformation_NetworkInterface::InitializeNetworkInterfaceSettings___VOID( CLR_RT_StackFrame& stack )
{
    NATIVE_PROFILE_CLR_NETWORK();
    NANOCLR_HEADER();

    HAL_Configuration_NetworkInterface config; 
    CLR_RT_HeapBlock* pConfig           = stack.Arg0().Dereference();  _ASSERTE(pConfig != NULL);
    CLR_UINT32 interfaceIndex           = pConfig[ FIELD___interfaceIndex ].NumericByRefConst().u4;

    NANOCLR_CLEAR(config);
    NANOCLR_CHECK_HRESULT(SOCK_CONFIGURATION_LoadAdapterConfiguration( interfaceIndex, &config ));

    pConfig[ FIELD___ipv4Address            ].SetInteger( (CLR_UINT32)config.IPv4Address);
    pConfig[ FIELD___ipv4GatewayAddress     ].SetInteger( (CLR_UINT32)config.IPv4GatewayAddress);
    pConfig[ FIELD___ipv4NetMask            ].SetInteger( (CLR_UINT32)config.IPv4NetMask);
    pConfig[ FIELD___ipv4dnsAddress1        ].SetInteger( (CLR_UINT32)config.IPv4DNSAddress1);
    pConfig[ FIELD___ipv4dnsAddress2        ].SetInteger( (CLR_UINT32)config.IPv4DNSAddress2);

    // FIXME IPV6
    // pConfig[ FIELD___ipv6Address            ].SetInteger( (CLR_UINT32)config.IPv6Address);
    // pConfig[ FIELD___ipv6GatewayAddress     ].SetInteger( (CLR_UINT32)config.IPv6GatewayAddress);
    // pConfig[ FIELD___ipv6NetMask            ].SetInteger( (CLR_UINT32)config.IPv6NetMask);
    // pConfig[ FIELD___ipv6dnsAddress1        ].SetInteger( (CLR_UINT32)config.IPv6DNSAddress1);
    // pConfig[ FIELD___ipv6dnsAddress2        ].SetInteger( (CLR_UINT32)config.IPv6DNSAddress2);

    // FIXME pConfig[ FIELD___networkInterfaceType   ].SetInteger( (CLR_UINT32)config.networkInterfaceType);

    NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_Array::CreateInstance( pConfig[ FIELD___macAddress ], NETIF_MAX_HWADDR_LEN, g_CLR_RT_WellKnownTypes.m_UInt8 ));   
    memcpy( pConfig[ FIELD___macAddress ].DereferenceArray()->GetFirstElement(), config.MacAddress, NETIF_MAX_HWADDR_LEN );
    
    NANOCLR_NOCLEANUP();
}

HRESULT Library_sys_net_native_System_Net_NetworkInformation_NetworkInterface::UpdateConfiguration___VOID__I4( CLR_RT_StackFrame& stack )
{
    NATIVE_PROFILE_CLR_NETWORK();
    NANOCLR_HEADER();

    HAL_Configuration_NetworkInterface config; 
    CLR_RT_HeapBlock* pConfig           = stack.Arg0().Dereference();  _ASSERTE(pConfig != NULL);
    CLR_UINT32 interfaceIndex           = pConfig[ FIELD___interfaceIndex ].NumericByRefConst().u4;
    CLR_UINT32 updateFlags              = stack.Arg1().NumericByRef().u4;
    CLR_RT_HeapBlock_Array* pMACAddress = pConfig[ FIELD___macAddress ].DereferenceArray();

    NANOCLR_CLEAR(config);

    config.IPv4Address           = pConfig[ FIELD___ipv4Address        ].NumericByRef().u4;
    config.IPv4GatewayAddress    = pConfig[ FIELD___ipv4GatewayAddress ].NumericByRef().u4;
    config.IPv4NetMask           = pConfig[ FIELD___ipv4NetMask        ].NumericByRef().u4;
    config.IPv4DNSAddress1       = pConfig[ FIELD___ipv4dnsAddress1    ].NumericByRef().u4;
    config.IPv4DNSAddress2       = pConfig[ FIELD___ipv4dnsAddress2    ].NumericByRef().u4;

    // FIXME IPV6
    // config.IPv6Address           = pConfig[ FIELD___ipv6Address        ].NumericByRef().u4;
    // config.IPv6GatewayAddress    = pConfig[ FIELD___ipv6GatewayAddress ].NumericByRef().u4;
    // config.IPv6NetMask           = pConfig[ FIELD___ipv6NetMask        ].NumericByRef().u4;
    // config.IPv6DNSAddress1       = pConfig[ FIELD___ipv6dnsAddress1    ].NumericByRef().u4;
    // config.IPv6DNSAddress2       = pConfig[ FIELD___ipv6dnsAddress2    ].NumericByRef().u4;

    // set network interface type 
    switch((NetworkInterfaceType)pConfig[ FIELD___networkInterfaceType ].NumericByRef().u4)
    {
        case NetworkInterfaceType_Ethernet:
            //memcpy( &config.MacAddress, pMACAddress->GetFirstElement(), NETIF_MAX_HWADDR_LEN ); 
            break;

        case NetworkInterfaceType_Wireless80211:
            break;
    }
    // FIXME config.networkInterfaceType = pConfig[ FIELD___networkInterfaceType ].NumericByRef().u4;
    
    if(pMACAddress != NULL)
    {
        memcpy( &config.MacAddress, pMACAddress->GetFirstElement(), NETIF_MAX_HWADDR_LEN ); 
    }

    NANOCLR_CHECK_HRESULT(SOCK_CONFIGURATION_UpdateAdapterConfiguration( &config, DeviceConfigurationOption_Network, interfaceIndex, updateFlags ));

    NANOCLR_NOCLEANUP();
}

HRESULT Library_sys_net_native_System_Net_NetworkInformation_NetworkInterface::GetNetworkInterfaceCount___STATIC__I4( CLR_RT_StackFrame& stack )
{
    NATIVE_PROFILE_CLR_NETWORK();
    NANOCLR_HEADER();

    stack.SetResult_I4( SOCK_CONFIGURATION_GetAdapterCount() );    

    NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_sys_net_native_System_Net_NetworkInformation_NetworkInterface::GetNetworkInterface___STATIC__SystemNetNetworkInformationNetworkInterface__U4( CLR_RT_StackFrame& stack )
{
    NATIVE_PROFILE_CLR_NETWORK();
    NANOCLR_HEADER();

    HAL_Configuration_NetworkInterface config; 
    CLR_UINT32                type = 0;
    CLR_RT_HeapBlock*         pConfig;
    CLR_UINT32                interfaceIndex = stack.Arg0().NumericByRef().u4;
    CLR_RT_HeapBlock&         top            = stack.PushValueAndClear();

    NANOCLR_CLEAR(config);

    NANOCLR_CHECK_HRESULT(SOCK_CONFIGURATION_LoadConfiguration( interfaceIndex, &config ));    

    // FIXME        
    // type = SOCK_NETWORKCONFIGURATION_FLAGS_TYPE__value(config.flags);
    
    // switch(type)
    // {
    //     case SOCK_NETWORKCONFIGURATION_FLAGS_NETWORK_INTERFACE:
    //         {
               NANOCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex( top, g_CLR_RT_WellKnownTypes.m_NetworkInterface ));
    //             break;
    //         }
    //     case SOCK_NETWORKCONFIGURATION_FLAGS_WIRELESS:
    //         {
    //             NANOCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex( top, g_CLR_RT_WellKnownTypes.m_Wireless80211 ));
    //             break;
    //         }
    //     default:
    //         {
    //             /// Unknown type, not supported.
    //             NANOCLR_SET_AND_LEAVE(CLR_E_FAIL);
    //             break;
    //         }
    // }
    
    pConfig = top.Dereference(); FAULT_ON_NULL(pConfig);

    pConfig[ FIELD___ipv4Address            ].SetInteger( (CLR_UINT32)config.IPv4Address);
    pConfig[ FIELD___ipv4GatewayAddress     ].SetInteger( (CLR_UINT32)config.IPv4GatewayAddress);
    pConfig[ FIELD___ipv4NetMask            ].SetInteger( (CLR_UINT32)config.IPv4NetMask);
    pConfig[ FIELD___ipv4dnsAddress1        ].SetInteger( (CLR_UINT32)config.IPv4DNSAddress1);
    pConfig[ FIELD___ipv4dnsAddress2        ].SetInteger( (CLR_UINT32)config.IPv4DNSAddress2);

    // FIXME IPV6
    // pConfig[ FIELD___ipv6Address            ].SetInteger( (CLR_UINT32)config.IPv6Address);
    // pConfig[ FIELD___ipv6GatewayAddress     ].SetInteger( (CLR_UINT32)config.IPv6GatewayAddress);
    // pConfig[ FIELD___ipv6NetMask            ].SetInteger( (CLR_UINT32)config.IPv6NetMask);
    // pConfig[ FIELD___ipv6dnsAddress1        ].SetInteger( (CLR_UINT32)config.IPv6DNSAddress1);
    // pConfig[ FIELD___ipv6dnsAddress2        ].SetInteger( (CLR_UINT32)config.IPv6DNSAddress2);

    // FIXME pConfig[ FIELD___networkInterfaceType   ].SetInteger( (CLR_UINT32)config.networkInterfaceType);

    NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_Array::CreateInstance( pConfig[ FIELD___macAddress ], NETIF_MAX_HWADDR_LEN, g_CLR_RT_WellKnownTypes.m_UInt8 ));   

    memcpy( pConfig[ FIELD___macAddress ].DereferenceArray()->GetFirstElement(), config.MacAddress, NETIF_MAX_HWADDR_LEN );

    // Type specific load.
    switch(type)
    {
        case SOCK_NETWORKCONFIGURATION_FLAGS_WIRELESS:
            {
                // FIXME
                // CLR_UINT32 wirelessIndex = SOCK_NETWORKCONFIGURATION_FLAGS_SUBINDEX__value(config.flags);
                // HAL_Configuration_Wireless80211NetworkInterface wirelessConfig;
                
                // NANOCLR_CHECK_HRESULT(SOCK_CONFIGURATION_LoadWirelessConfiguration( wirelessIndex, &wirelessConfig ));

                // pConfig[Library_sys_net_native_System_Net_NetworkInformation_Wireless80211NetworkInterface::FIELD__Authentication ].SetInteger( (CLR_INT32)WIRELESS_FLAG_AUTHENTICATION__value(wirelessConfig.wirelessFlags) );
                // pConfig[Library_sys_net_native_System_Net_NetworkInformation_Wireless80211NetworkInterface::FIELD__Encryption     ].SetInteger( (CLR_INT32)WIRELESS_FLAG_ENCRYPTION__value    (wirelessConfig.wirelessFlags) );
                // pConfig[Library_sys_net_native_System_Net_NetworkInformation_Wireless80211NetworkInterface::FIELD__Radio          ].SetInteger( (CLR_INT32)WIRELESS_FLAG_RADIO__value         (wirelessConfig.wirelessFlags) );

                // wirelessConfig.passPhrase[ WIRELESS_PASSPHRASE_LENGTH - 1 ] = 0;
                // NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_String::CreateInstance( pConfig[Library_sys_net_native_System_Net_NetworkInformation_Wireless80211NetworkInterface::FIELD__Password ], wirelessConfig.passPhrase ));                

                // NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_String::CreateInstance( pConfig[Library_sys_net_native_System_Net_NetworkInformation_Wireless80211NetworkInterface::FIELD__Ssid ], wirelessConfig.ssid ));                

                break;
            }
        default:
            {
                break;
            }
    }

    NANOCLR_NOCLEANUP();
}

HRESULT Library_sys_net_native_System_Net_NetworkInformation_NetworkInterface::IPAddressFromString___STATIC__U4__STRING( CLR_RT_StackFrame& stack )
{
    NATIVE_PROFILE_CLR_NETWORK();
    NANOCLR_HEADER();

    LPCSTR szName = stack.Arg0().RecoverString();
    struct SOCK_addrinfo hints;
    struct SOCK_addrinfo* addr = NULL;     
    struct SOCK_sockaddr_in* addr_in;
    int ret;
    
    memset( &hints, 0, sizeof(hints) );

    ret = SOCK_getaddrinfo( szName, NULL, &hints, &addr );

    // getaddrinfo returns a winsock error code rather than SOCK_SOCKET_ERROR, so pass this on to the exception handling
    if(ret != 0 || addr == NULL || addr->ai_family != SOCK_AF_INET)
    {
        NANOCLR_SET_AND_LEAVE(CLR_E_FAIL);
    }

    _ASSERTE(addr->ai_addr != NULL);
    _ASSERTE(addr->ai_addrlen >= sizeof(SOCK_sockaddr_in));

    addr_in = (struct SOCK_sockaddr_in*)addr->ai_addr;

    stack.PushValue().SetInteger( (CLR_UINT32)addr_in->sin_addr.S_un.S_addr );

    NANOCLR_CLEANUP();

    if( addr ) SOCK_freeaddrinfo( addr );

    NANOCLR_CLEANUP_END();
}
