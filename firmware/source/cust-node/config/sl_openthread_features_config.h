//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------
//
// <h> OpenThread Stack Configurations
// <h>  Thread Version
// <o   OPENTHREAD_CONFIG_THREAD_VERSION> THREAD_VERSION
//      <2=> Thread 1.1
//      <3=> Thread 1.2
// <i>  Thread 1.2 is compatible with Thread 1.1.
// <i>  Current Default: 2 (Thread 1.1)
#define OPENTHREAD_CONFIG_THREAD_VERSION 2

// <e> Thread 1.2 Features
#define OPENTHREAD_VERSION_1_2_FEATURE_ENABLE    0
#if ((OPENTHREAD_CONFIG_THREAD_VERSION == 3) && OPENTHREAD_VERSION_1_2_FEATURE_ENABLE)
// <e>  Backbone Router
#define OPENTHREAD_CONFIG_BACKBONE_ROUTER_ENABLE    0
// </e>
// <e>  CSL (Coordinated Sampled Listening) Debug
#define OPENTHREAD_CONFIG_MAC_CSL_DEBUG_ENABLE      0
// </e>
// <e>  CSL (Coordinated Sampled Listening) Receiver
#define OPENTHREAD_CONFIG_MAC_CSL_RECEIVER_ENABLE   0
// </e>
// <e>  DUA (Domain Unicast Address)
#define OPENTHREAD_CONFIG_DUA_ENABLE                0
// </e>
// <e>  Link Metrics
#define OPENTHREAD_CONFIG_MLE_LINK_METRICS_ENABLE   0
// </e>
// <e>  Multicast Listener Registration
#define OPENTHREAD_CONFIG_MLR_ENABLE                0
// </e>
#endif
// </e>
// </h>

// <e>  Border Agent
#define OPENTHREAD_CONFIG_BORDER_AGENT_ENABLE       0
// </e>
// <e>  Border Router
#define OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE      1
// </e>
// <e>  Channel Manager
#define OPENTHREAD_CONFIG_CHANNEL_MANAGER_ENABLE    0
// </e>
// <e>  Channel Monitor
#define OPENTHREAD_CONFIG_CHANNEL_MONITOR_ENABLE    0
// </e>
// <e>  Child Supervision
#define OPENTHREAD_CONFIG_CHILD_SUPERVISION_ENABLE  0
// </e>
// <e>  Commissioner
#define OPENTHREAD_CONFIG_COMMISSIONER_ENABLE       1
// </e>
// <e>  COAP API
#define OPENTHREAD_CONFIG_COAP_API_ENABLE           1
// </e>
// <e>  COAP Observe (RFC7641) API
#define OPENTHREAD_CONFIG_COAP_OBSERVE_API_ENABLE   1
// </e>
// <e>  COAP Secure API
#define OPENTHREAD_CONFIG_COAP_SECURE_API_ENABLE    1
// </e>
// <e>  DHCP6 Client
#define OPENTHREAD_CONFIG_DHCP6_CLIENT_ENABLE       1
// </e>
// <e>  DHCP6 Server
#define OPENTHREAD_CONFIG_DHCP6_SERVER_ENABLE       1
// </e>
// <e>  Diagnostic
#define OPENTHREAD_CONFIG_DIAG_ENABLE               0
// </e>
// <e>  DNS Client
#define OPENTHREAD_CONFIG_DNS_CLIENT_ENABLE         0
// </e>
// <e>  ECDSA (Elliptic Curve Digital Signature Algorithm)
#define OPENTHREAD_CONFIG_ECDSA_ENABLE              0
// </e>
// <e>  External Heap
#define OPENTHREAD_CONFIG_HEAP_EXTERNAL_ENABLE      1
// </e>
// <e>  IPv6 Fragmentation
#define OPENTHREAD_CONFIG_IP6_FRAGMENTATION_ENABLE  0
// </e>
// <e>  Jam Detection
#define OPENTHREAD_CONFIG_JAM_DETECTION_ENABLE      0
// </e>
// <e>  Joiner
#define OPENTHREAD_CONFIG_JOINER_ENABLE             1
// </e>
// <e>  Legacy Network
#define OPENTHREAD_CONFIG_LEGACY_ENABLE             0
// </e>
// <e>  Link Raw Service
#define OPENTHREAD_CONFIG_LINK_RAW_ENABLE           0
// </e>
// <e>  MAC Filter
#define OPENTHREAD_CONFIG_MAC_FILTER_ENABLE         0
// </e>
// <e>  MLE Long Routes extension (experimental)
#define OPENTHREAD_CONFIG_MLE_LONG_ROUTES_ENABLE    0
// </e>
// <e>  Multiple OpenThread Instances
#define OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE      0
// </e>
// <e>  OTNS (OpenThread Network Simulator)
#define OPENTHREAD_CONFIG_OTNS_ENABLE               0
// </e>
// <e>  Platform UDP
#define OPENTHREAD_CONFIG_PLATFORM_UDP_ENABLE       0
// </e>
// <e>  Reference Device for Thread Test Harness
#define OPENTHREAD_CONFIG_REFERENCE_DEVICE_ENABLE   1
// </e>
// <e>  Service Entries in Thread Network Data
#define OPENTHREAD_CONFIG_TMF_NETDATA_SERVICE_ENABLE    0
// </e>
// <e>  RAM (volatile-only storage)
#define OPENTHREAD_SETTINGS_RAM                     0
// </e>
// <e>  SLAAC Addresses
#define OPENTHREAD_CONFIG_IP6_SLAAC_ENABLE          1
// </e>
// <e>  SNTP Client
#define OPENTHREAD_CONFIG_SNTP_CLIENT_ENABLE        0
// </e>
// <e>  Time Synchronization Service
#define OPENTHREAD_CONFIG_TIME_SYNC_ENABLE          0
// </e>
// <e>  TMF Network Diagnostics for MTD
#define OPENTHREAD_CONFIG_TMF_NETWORK_DIAG_MTD_ENABLE   0
// </e>
#if OPENTHREAD_CONFIG_TIME_SYNC_ENABLE || (OPENTHREAD_CONFIG_THREAD_VERSION >= OT_THREAD_VERSION_1_2)
#define OPENTHREAD_CONFIG_MAC_HEADER_IE_SUPPORT     1
#endif

// <e>  UDP Forward
#define OPENTHREAD_CONFIG_UDP_FORWARD_ENABLE        0
// </e>
// </h>
// <h>  Logging
// <o   OPENTHREAD_CONFIG_LOG_OUTPUT> LOG_OUTPUT
//      <OPENTHREAD_CONFIG_LOG_OUTPUT_NONE             => NONE
//      <OPENTHREAD_CONFIG_LOG_OUTPUT_APP              => APP
//      <OPENTHREAD_CONFIG_LOG_OUTPUT_PLATFORM_DEFINED => PLATFORM_DEFINED
// <i>  Default: OPENTHREAD_CONFIG_LOG_OUTPUT_PLATFORM_DEFINED
#define OPENTHREAD_CONFIG_LOG_OUTPUT OPENTHREAD_CONFIG_LOG_OUTPUT_APP

// <q   OPENTHREAD_CONFIG_LOG_LEVEL_DYNAMIC_ENABLE> DYNAMIC_LOG_LEVEL
#define OPENTHREAD_CONFIG_LOG_LEVEL_DYNAMIC_ENABLE  0

// <e>  Enable Logging
#define OPENTHREAD_FULL_LOGS_ENABLE                 0
#if     OPENTHREAD_FULL_LOGS_ENABLE

// <o   OPENTHREAD_CONFIG_LOG_LEVEL> LOG_LEVEL
//      <OT_LOG_LEVEL_NONE       => NONE
//      <OT_LOG_LEVEL_CRIT       => CRIT
//      <OT_LOG_LEVEL_WARN       => WARN
//      <OT_LOG_LEVEL_NOTE       => NOTE
//      <OT_LOG_LEVEL_INFO       => INFO
//      <OT_LOG_LEVEL_DEBG       => DEBG
// <i>  Default: OT_LOG_LEVEL_DEBG
#define OPENTHREAD_CONFIG_LOG_LEVEL OT_LOG_LEVEL_DEBG

// <q   OPENTHREAD_CONFIG_LOG_API>                  API
#define OPENTHREAD_CONFIG_LOG_API                   1
// <q   OPENTHREAD_CONFIG_LOG_ARP>                  ARP
#define OPENTHREAD_CONFIG_LOG_ARP                   1
// <q   OPENTHREAD_CONFIG_LOG_BBR>                  BBR
#define OPENTHREAD_CONFIG_LOG_BBR                   1
// <q   OPENTHREAD_CONFIG_LOG_CLI>                  CLI
#define OPENTHREAD_CONFIG_LOG_CLI                   1
// <q   OPENTHREAD_CONFIG_LOG_COAP>                 COAP
#define OPENTHREAD_CONFIG_LOG_COAP                  1
// <q   OPENTHREAD_CONFIG_LOG_DUA>                  DUA
#define OPENTHREAD_CONFIG_LOG_DUA                   1
// <q   OPENTHREAD_CONFIG_LOG_ICMP>                 ICMP
#define OPENTHREAD_CONFIG_LOG_ICMP                  1
// <q   OPENTHREAD_CONFIG_LOG_IP6>                  IP6
#define OPENTHREAD_CONFIG_LOG_IP6                   1
// <q   OPENTHREAD_CONFIG_LOG_MAC>                  MAC
#define OPENTHREAD_CONFIG_LOG_MAC                   1
// <q   OPENTHREAD_CONFIG_LOG_MEM>                  MEM
#define OPENTHREAD_CONFIG_LOG_MEM                   1
// <q   OPENTHREAD_CONFIG_LOG_MESHCOP>              MESHCOP
#define OPENTHREAD_CONFIG_LOG_MESHCOP               1
// <q   OPENTHREAD_CONFIG_LOG_MLE>                  MLE
#define OPENTHREAD_CONFIG_LOG_MLE                   1
// <q   OPENTHREAD_CONFIG_LOG_MLR>                  MLR
#define OPENTHREAD_CONFIG_LOG_MLR                   1
// <q   OPENTHREAD_CONFIG_LOG_NETDATA>              NETDATA
#define OPENTHREAD_CONFIG_LOG_NETDATA               1
// <q   OPENTHREAD_CONFIG_LOG_NETDIAG>              NETDIAG
#define OPENTHREAD_CONFIG_LOG_NETDIAG               1
// <q   OPENTHREAD_CONFIG_LOG_PKT_DUMP>             PKT_DUMP
#define OPENTHREAD_CONFIG_LOG_PKT_DUMP              1
// <q   OPENTHREAD_CONFIG_LOG_PLATFORM>             PLATFORM
#define OPENTHREAD_CONFIG_LOG_PLATFORM              1
// <q   OPENTHREAD_CONFIG_LOG_PREPEND_LEVEL>        PREPEND_LEVEL
#define OPENTHREAD_CONFIG_LOG_PREPEND_LEVEL         1
// <q   OPENTHREAD_CONFIG_LOG_PREPEND_REGION>       PREPEND_REGION
#define OPENTHREAD_CONFIG_LOG_PREPEND_REGION        1
#endif
// </e>
// </h>

// <<< end of configuration section >>>
