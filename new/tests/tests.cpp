//
// Created by 79933 on 2026/1/9.
//

#include "gtest/gtest.h"
#ifdef _WIN32
#include "libTSCAN.h"
#else
#include "TSCANDef.hpp"
#endif

#include <iostream>

TEST(test, test){
    std::cout << "";
}

TEST(test, tosuncan){
    initialize_lib_tscan(true, true, true);

    uint32_t count = 0;
    std::cout << tscan_scan_devices(&count) << std::endl;
    std::cout << "count: " << count << std::endl;

    char manufacturer[100]{};
    char product[100]{};
    char serial[100]{};
    char dvc_name[100]{};
    char *pManufacturer = manufacturer;
    char *pProduct = product;
    char *pSerial = serial;
    char *pDvc_name = dvc_name;
    int32_t dvc_type;
    int32_t canCount;
    bool isFD;
    int32_t linCount;
    int32_t frCount;
    int32_t ethCount;

    std::cout << "get device information detail, result: " << tscan_get_device_info_detail(0, &pManufacturer, &pProduct, &pSerial, &dvc_type, &pDvc_name, &canCount, &isFD, &linCount, &frCount, &ethCount) << std::endl;
    std::cout << "manufacturer: " << pManufacturer << " product: " << pProduct << " serial: " << pSerial << std::endl;
    std::cout << "device name: " << pDvc_name << " device type: " << dvc_type << std::endl;
    std::cout << "can counts: " << canCount << " is FD: " << isFD << " lin count: " << linCount << " flexray count: " << frCount << " ether count: " << ethCount << std::endl;

    uint64_t handle0 = 0;
    std::cout << "device connect, result: " << tscan_connect(pSerial, &handle0) << std::endl;

    std::cout << "get device information detail, result: " << tscan_get_device_info_detail(1, &pManufacturer, &pProduct, &pSerial, &dvc_type, &pDvc_name, &canCount, &isFD, &linCount, &frCount, &ethCount) << std::endl;
    std::cout << "manufacturer: " << pManufacturer << " product: " << pProduct << " serial: " << pSerial << std::endl;
    std::cout << "device name: " << pDvc_name << " device type: " << dvc_type << std::endl;
    std::cout << "can counts: " << canCount << " is FD: " << isFD << " lin count: " << linCount << " flexray count: " << frCount << " ether count: " << ethCount << std::endl;

    uint64_t handle1 = 0;
    std::cout << "device connect, result: " << tscan_connect(pSerial, &handle1);

    std::cout << "set baudrate, result:" << tscan_config_can_by_baudrate(handle0, CHN1, 500, 1) << std::endl;
    std::cout << "set baudrate, result:" << tscan_config_can_by_baudrate(handle1, CHN1, 500, 1) << std::endl;

    TLibCAN canFrame{};
    canFrame.FIdentifier = 0x03; // Frame id
    canFrame.FProperties = 0x00; // not remote frame,standard frame
    canFrame.FDLC = 8;           // Frame DLC
    canFrame.FIdxChn = CHN1;
    for(int i = 0; i < 8; i++)
        canFrame.FData[i] = i;

    for(int i = 0; i < 10; i++){
        tscan_transmit_can_sync(handle0, &canFrame, 100);
        tscan_transmit_can_sync(handle1, &canFrame, 100);
        usleep(100000);
//        std::cout << "set can frame, result: " << tscan_transmit_can_sync(handle0, &canFrame, 100) << std::endl;
//        std::cout << "set can frame, result: " << tscan_transmit_can_sync(handle1, &canFrame, 100) << std::endl;
    }

    TLibCAN canFrameBuffer[10];
    int32_t size = 10;
    std::cout << "get can frame, result: " << tsfifo_receive_can_msgs(handle1, canFrameBuffer, &size, CHN1, 1) << std::endl;

    for(int i = 0; i < 10; i++) {
        std::cout << "chn: " << static_cast<int>(canFrameBuffer[i].FIdxChn) << std::endl;
        std::cout << "data: ";
        for(int j = 0; j < 8; j++)
            std::cout << static_cast<int>(canFrameBuffer[i].FData[j]) << ' ';
        std::cout << std::endl;
    }

    std::cout << "device disconnect, result: " << tscan_disconnect_by_handle(handle0);
    std::cout << "device disconnect, result: " << tscan_disconnect_by_handle(handle1);
}
