#pragma once
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include "xrt/xrt_kernel.h"
#include "xrt/xrt_bo.h"
#include "xrt/xrt_device.h"

class FPGA_ORB {
public:
  // 固定 640x480，max_kp 默认 700（与 HLS MAX_KEYPOINTS 一致）
  FPGA_ORB(const std::string &xclbin_path, int max_kp = 2000);
  ~FPGA_ORB();

  bool isReady() const;
  
  // 提取函数：输入必须是 640x480 灰度图
  bool extract(const cv::Mat &gray, 
               std::vector<cv::KeyPoint> &out_kp, 
               cv::Mat &out_desc, 
               unsigned int threshold=14);

private:
  bool initialized;
  xrt::device device;
  xrt::uuid uuid;
  xrt::kernel krnl;

  xrt::bo bo_img, bo_kp, bo_desc, bo_nk;
  uint8_t  *p_img;
  uint8_t  *p_kp;
  uint8_t  *p_desc;
  uint16_t *p_nk;

  size_t max_keypoints;
  static constexpr size_t IMG_WIDTH = 640;
  static constexpr size_t IMG_HEIGHT = 480;
  static constexpr size_t KP_ELEM_BYTES = 8;
};
