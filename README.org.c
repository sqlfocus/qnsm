本文档摘录qnsm的主流程，主要关注DPDK和suricata的结合


--app_config_parse()                     解析配置文件
--app_init()                             组件初始化
--qnsm_service_lib_init()
  --qnsm_idps_init()                     初始化suricata, 设置运行模式 RUNMODE_DPDK
--app_lcore_main_loop()
  --qnsm_servcie_app_launch()            启动各核心对应的服务
    --qnsm_crm_agent_init()
    --qnsm_msg_init()
    --qnsm_tbl_init()
    --qnsm_dpi_init()
    --qnsm_port_service_init()
    --qnsm_kafka_init()
    --qnsm_acl_init()
    --QNSM_DATA->init_fun()
    ==>qnsm_detect_service_init()        服务初始化, EN_QNSM_DETECT
      --QnsmTmThreadsInit()
        --QnsmTmThreadCreatePacketHandler()
          --TmThreadCreate()               创建线程环境(suricata函数)
            --输入队列: "packetpool"
            --输出队列: "packetpool"
            --自定义主入口: "custom" -> QnsmTmThreadsSlotPktAcqLoop()
        --TmModuleReceiveQnsmRegister()    注册 tmm_modules[TMM_RECEIVEDPDK]
                                           注册 tmm_modules[TMM_DECODEDPDK]
        --TmSlotSetFuncAppend()            注册报文处理函数链
          --"QnsmReceive"
          --"QnsmDecode"
          --"FlowWorker"
          --"RespondReject"
      --qnsm_service_run_reg()             注册服务运行函数
    --QNSM_DATA->service_run()           运行服务, EN_QNSM_DETECT
    ==>qnsm_detect_run()
      --QnsmTMThreadsRun()
        --ThreadVars->tm_func()
        ==>QnsmTmThreadsSlotPktAcqLoop()


* DETECT线程主处理入口
此线程借助suricata库，实现了IDS的功能
    
--QnsmTmThreadsSlotPktAcqLoop()
  --QnsmTMReceiveLoop()
    --qnsm_port_rx()                     收包
    --QnsmTMConstructPacket()            构建报文
    --TmThreadsSlotProcessPkt()          调用其他报文处理函数链
