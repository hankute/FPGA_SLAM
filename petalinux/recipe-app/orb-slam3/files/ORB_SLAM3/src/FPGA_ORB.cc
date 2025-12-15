#include "FPGA_ORB.h"
#include <iostream>
#include <cstring>

#include <chrono>

FPGA_ORB::FPGA_ORB(const std::string &xclbin_path, int max_kp) {
  initialized = false;
  max_keypoints = max_kp;
  
  try {
    device = xrt::device(0);
    uuid   = device.load_xclbin(xclbin_path);
    krnl   = xrt::kernel(device, uuid, "orb_feature_extraction");

    size_t img_bytes = IMG_WIDTH * IMG_HEIGHT;
    size_t kp_bytes = max_keypoints * KP_ELEM_BYTES;
    size_t desc_bytes = max_keypoints * 32;

    bo_img  = xrt::bo(device, img_bytes,     krnl.group_id(0));
    bo_kp   = xrt::bo(device, kp_bytes,      krnl.group_id(1));
    bo_desc = xrt::bo(device, desc_bytes,    krnl.group_id(2));
    bo_nk   = xrt::bo(device, sizeof(uint16_t), krnl.group_id(1));

    p_img  = bo_img.map<uint8_t*>();
    p_kp   = bo_kp.map<uint8_t*>();
    p_desc = bo_desc.map<uint8_t*>();
    p_nk   = bo_nk.map<uint16_t*>();

    initialized = true;
    std::cout << "========================================" << std::endl;
    std::cout << "✓ FPGA_ORB Initialized" << std::endl;
    std::cout << "  Target size: 640x480 (auto-resize)" << std::endl;
    std::cout << "  Max keypoints: " << max_keypoints << std::endl;
    std::cout << "========================================" << std::endl;
  } catch(const std::exception &e) {
    std::cerr << "✗ FPGA_ORB init error: " << e.what() << std::endl;
    initialized = false;
  }
}

FPGA_ORB::~FPGA_ORB() {}

bool FPGA_ORB::isReady() const { 
  return initialized; 
}

// bool FPGA_ORB::extract(const cv::Mat &gray_input, 
//                        std::vector<cv::KeyPoint> &out_kp, 
//                        cv::Mat &out_desc, 
//                        unsigned int threshold) {
//   if(!initialized) {
//     std::cerr << "✗ FPGA not initialized" << std::endl;
//     return false;
//   }

//   if(gray_input.type() != CV_8UC1) {
//     std::cerr << "✗ Input must be CV_8UC1" << std::endl;
//     return false;
//   }

//   // ✅ 自动 Resize 到 640x480
//   cv::Mat gray_640x480;
//   float scale_x = 1.0f, scale_y = 1.0f;
  
//   if(gray_input.cols != (int)IMG_WIDTH || gray_input.rows != (int)IMG_HEIGHT) {
//     scale_x = (float)gray_input.cols / (float)IMG_WIDTH;
//     scale_y = (float)gray_input.rows / (float)IMG_HEIGHT;
//             return false;
//     // cv::resize(gray_input, gray_640x480, cv::Size(IMG_WIDTH, IMG_HEIGHT), 
//     //            0, 0, cv::INTER_LINEAR);
//     // std::cout << "ℹ [FPGA] Resized from " << gray_input.cols << "x" << gray_input.rows 
//     //           << " to 640x480 (scale_x=" << scale_x << ", scale_y=" << scale_y << ")" << std::endl;
//   } else {
//     gray_640x480 = gray_input;
//   }

//   // 确保内存连续
//   if(!gray_640x480.isContinuous()) {
//     gray_640x480 = gray_640x480.clone();
//   }

//   try {
//     // 拷贝到 device
//     memcpy(p_img, gray_640x480.data, IMG_WIDTH * IMG_HEIGHT);
//     *p_nk = 0;
    
//     bo_img.sync(XCL_BO_SYNC_BO_TO_DEVICE);
//     bo_nk.sync(XCL_BO_SYNC_BO_TO_DEVICE);

//     // 执行 kernel
//     auto run = krnl(bo_img, bo_kp, bo_desc, bo_nk, (uint8_t)threshold);
//     run.wait();

//     // 读回结果
//     bo_nk.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
//     uint16_t numkp = *p_nk;

//     if(numkp > max_keypoints) {
//       std::cerr << "⚠ Truncating " << numkp << " to " << max_keypoints << std::endl;
//       numkp = max_keypoints;
//     }

//     if(numkp == 0) {
//       out_kp.clear();
//       out_desc = cv::Mat();
//       std::cout << "ℹ [FPGA] No keypoints detected" << std::endl;
//       return true;
//     }

//     bo_kp.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
//     bo_desc.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

//     // ✅ 解析 KeyPoints 并缩放坐标回原图
//     out_kp.clear();
//     out_kp.reserve(numkp);

//     for(uint16_t i = 0; i < numkp; i++) {
//       uint8_t *base = p_kp + i * KP_ELEM_BYTES;
      
//       int16_t  x_fpga    = *reinterpret_cast<int16_t*>(base + 0);
//       int16_t  y_fpga    = *reinterpret_cast<int16_t*>(base + 2);
//       uint16_t response  = *reinterpret_cast<uint16_t*>(base + 4);
//       uint8_t  octave    = *(base + 6);

//       cv::KeyPoint kp;
//       // ✅ 缩放坐标回原图尺寸
//       kp.pt.x = static_cast<float>(x_fpga) * scale_x;
//       kp.pt.y = static_cast<float>(y_fpga) * scale_y;
//       kp.response = static_cast<float>(response);
//       kp.octave = 0;  // Level 0
//       kp.size = 7.0f;
//       kp.angle = -1.0f;
      
//       out_kp.push_back(kp);
//     }

//     // ✅ 直接使用 FPGA 描述子（256 bits = 32 bytes）
//     out_desc = cv::Mat(numkp, 32, CV_8U);
//     memcpy(out_desc.data, p_desc, numkp * 32);

//     std::cout << "✓ [FPGA] Extracted " << numkp << " keypoints" << std::endl;
//     return true;

//   } catch(const std::exception &e) {
//     std::cerr << "✗ FPGA execution error: " << e.what() << std::endl;
//     return false;
//   }
// }

bool FPGA_ORB::extract(const cv::Mat &gray_input, 
                       std::vector<cv::KeyPoint> &out_kp, 
                       cv::Mat &out_desc, 
                       unsigned int threshold) 
{
    if(!initialized) {
        std::cerr << "✗ FPGA not initialized" << std::endl;
        return false;
    }

    if(gray_input.type() != CV_8UC1) {
        std::cerr << "✗ Input must be CV_8UC1" << std::endl;
        return false;
    }

    // ✅ 总计时开始
    auto t_total_start = std::chrono::high_resolution_clock::now();

    // Resize 到 640x480
    cv::Mat gray_640x480;
    float scale_x = 1.0f, scale_y = 1.0f;
    
    if(gray_input.cols != (int)IMG_WIDTH || gray_input.rows != (int)IMG_HEIGHT) {
        scale_x = (float)gray_input.cols / (float)IMG_WIDTH;
        scale_y = (float)gray_input.rows / (float)IMG_HEIGHT;
        
        auto t_resize_start = std::chrono::high_resolution_clock::now();
        cv::resize(gray_input, gray_640x480, cv::Size(IMG_WIDTH, IMG_HEIGHT), 0, 0, cv::INTER_LINEAR);
        auto t_resize_end = std::chrono::high_resolution_clock::now();
        double resize_ms = std::chrono::duration_cast<std::chrono::microseconds>(t_resize_end - t_resize_start).count() / 1000.0;
        // std::cout << "  Resize: " << std::endl;
    } else {
        gray_640x480 = gray_input;
    }

    if(!gray_640x480.isContinuous()) {
        gray_640x480 = gray_640x480.clone();
    }

    try {
        // ✅ 1. 拷贝到 buffer
        // auto t1 = std::chrono::high_resolution_clock::now();
        memcpy(p_img, gray_640x480.data, IMG_WIDTH * IMG_HEIGHT);
        *p_nk = 0;
        // auto t2 = std::chrono::high_resolution_clock::now();
        // double copy_ms = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.0;

        // ✅ 2. 同步到设备
        // auto t3 = std::chrono::high_resolution_clock::now();
        bo_img.sync(XCL_BO_SYNC_BO_TO_DEVICE);
        bo_nk.sync(XCL_BO_SYNC_BO_TO_DEVICE);
        // auto t4 = std::chrono::high_resolution_clock::now();
        // double sync_to_ms = std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count() / 1000.0;

        // ✅ 3. 执行 kernel
        // auto t5 = std::chrono::high_resolution_clock::now();
        auto run = krnl(bo_img, bo_kp, bo_desc, bo_nk, (uint8_t)threshold);
        // auto t6 = std::chrono::high_resolution_clock::now();
        // double kernel_submit_ms = std::chrono::duration_cast<std::chrono::microseconds>(t6 - t5).count() / 1000.0;
        
        // ✅ 4. 等待完成
        // auto t7 = std::chrono::high_resolution_clock::now();
        run.wait();
        // auto t8 = std::chrono::high_resolution_clock::now();
        // double kernel_wait_ms = std::chrono::duration_cast<std::chrono::microseconds>(t8 - t7).count() / 1000.0;

        // ✅ 5. 同步回主机
        // auto t9 = std::chrono::high_resolution_clock::now();
        bo_nk.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
        uint16_t numkp = *p_nk;
        // auto t10 = std::chrono::high_resolution_clock::now();
        // double sync_nk_ms = std::chrono::duration_cast<std::chrono::microseconds>(t10 - t9).count() / 1000.0;

        if(numkp > max_keypoints) {
            numkp = max_keypoints;
        }

        if(numkp == 0) {
            out_kp.clear();
            out_desc = cv::Mat();
            
            // auto t_total_end = std::chrono::high_resolution_clock::now();
            // double total_ms = std::chrono::duration_cast<std::chrono::microseconds>(t_total_end - t_total_start).count() / 1000.0;
            
            // std::cout << "  [Timing] No keypoints, total: " << total_ms << " ms" << std::endl;
            return true;
        }

        // ✅ 6. 同步 keypoints 和 descriptors
        // auto t11 = std::chrono::high_resolution_clock::now();

        bo_kp.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
        // bo_desc.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

        // auto t12 = std::chrono::high_resolution_clock::now();
        // double sync_results_ms = std::chrono::duration_cast<std::chrono::microseconds>(t12 - t11).count() / 1000.0;

        // ✅ 7. 解析结果
        // auto t13 = std::chrono::high_resolution_clock::now();
        out_kp.clear();
        out_kp.reserve(numkp);
        ///////////////////////////////////////////
// 用金字塔0层（传进来的 gray_input）的尺寸做边界检查
const int BORDER = 16;                  // ORB 的 31×31 patch 半径，通常为 15
const int width  = gray_input.cols;
const int height = gray_input.rows;
        ///////////////////////////////////////////
        for(uint16_t i = 0; i < numkp; i++) {
            uint8_t *base = p_kp + i * KP_ELEM_BYTES;
            
            int16_t  x_fpga    = *reinterpret_cast<int16_t*>(base + 0);
            int16_t  y_fpga    = *reinterpret_cast<int16_t*>(base + 2);
            uint16_t response  = *reinterpret_cast<uint16_t*>(base + 4);
            uint8_t  octave    = *(base + 6);
////////////////////////////////////////////////////////////////////////////////////
    float x_img = static_cast<float>(x_fpga) * scale_x;
    float y_img = static_cast<float>(y_fpga) * scale_y;

    // ⭐ 和 IC_Angle 完全一致的安全边界：以 cvRound 后的点为中心，要能放下半径 BORDER 的 patch
    int ix = cvRound(x_img);
    int iy = cvRound(y_img);
    if (ix < BORDER || ix >= width - BORDER ||
        iy < BORDER || iy >= height - BORDER) {
        continue; // 丢弃靠边/越界的点，避免 computeOrientation 里 segfault
    }
////////////////////////////////////////////////////////////////////////////////////
            cv::KeyPoint kp;
            kp.pt.x = static_cast<float>(x_fpga) * scale_x;
            kp.pt.y = static_cast<float>(y_fpga) * scale_y;
            kp.response = static_cast<float>(response);
            kp.octave = 0;
            kp.size = 31.0f;
            kp.angle = -1.0f;
            
            out_kp.push_back(kp);
        }

        // out_desc = cv::Mat(numkp, 32, CV_8U);
        // memcpy(out_desc.data, p_desc, numkp * 32);
        out_desc.release(); 
        // auto t14 = std::chrono::high_resolution_clock::now();
        // double parse_ms = std::chrono::duration_cast<std::chrono::microseconds>(t14 - t13).count() / 1000.0;

        // ✅ 总时间
        // auto t_total_end = std::chrono::high_resolution_clock::now();
        // double total_ms = std::chrono::duration_cast<std::chrono::microseconds>(t_total_end - t_total_start).count() / 1000.0;

        // // ✅ 详细输出
        // std::cout << "\n[FPGA Timing Breakdown - " << numkp << " keypoints]" << std::endl;
        // std::cout << "  1. Copy to buffer:   " << std::fixed << std::setprecision(2) << copy_ms << " ms" << std::endl;
        // std::cout << "  2. Sync to device:   " << sync_to_ms << " ms  ";
        // if(sync_to_ms > 10) std::cout << "❌ SLOW!";
        // std::cout << std::endl;
        
        // std::cout << "  3. Kernel submit:    " << kernel_submit_ms << " ms" << std::endl;
        // std::cout << "  4. Kernel wait:      " << kernel_wait_ms << " ms  ";
        // if(kernel_wait_ms > 10) std::cout << "❌ SLOW!";
        // std::cout << std::endl;
        
        // std::cout << "  5. Sync num_kp:      " << sync_nk_ms << " ms" << std::endl;
        // std::cout << "  6. Sync results:     " << sync_results_ms << " ms" << std::endl;
        // std::cout << "  7. Parse results:    " << parse_ms << " ms" << std::endl;
        // std::cout << "  -----------------------------------" << std::endl;
        // std::cout << "  TOTAL:               " << total_ms << " ms" << std::endl;
        
        // // 标记异常慢的情况
        // if(total_ms > 20) {
        //     std::cout << "  ⚠️  WARNING: Abnormally slow!" << std::endl;
        // }
        // std::cout << std::endl;

        return true;

    } catch(const std::exception &e) {
        std::cerr << "✗ FPGA execution error: " << e.what() << std::endl;
        return false;
    }
}



