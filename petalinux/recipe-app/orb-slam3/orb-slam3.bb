SUMMARY = "ORB-SLAM3: An Accurate Open-Source Library for Visual, Visual-Inertial and Multi-Map SLAM"
LICENSE = "GPLv3"
LIC_FILES_CHKSUM = "file://LICENSE;md5=84dcc94da3adb52b53ae4fa38fe49e5d"

SRC_URI = "file://ORB_SLAM3/ \
           file://disable-march-native.patch \
"

S = "${WORKDIR}/ORB_SLAM3"

inherit cmake externalsrc

EXTERNALSRC = "${THISDIR}/files/ORB_SLAM3"

# >>> 修改点：把 dbow2/g2o 加入依赖（如果你 later 添加独立 recipe，可在这里保留）
DEPENDS = "opencv libeigen boost pangolin glew xrt"
# <<< 修改点结束

EXTRA_OECMAKE += "\
    -DCMAKE_BUILD_TYPE=Release \
    -DORB_SLAM3_BUILD_EXAMPLES=ON \
    -DORB_SLAM3_BUILD_TESTS=OFF \
    -DG2O_DIR=${RECIPE_SYSROOT}/usr/lib/cmake/g2o \
    -DCMAKE_PREFIX_PATH=${RECIPE_SYSROOT}/usr \
    -DCMAKE_SYSROOT=${RECIPE_SYSROOT} \
    -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
    -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
    -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=${S}/lib \
    -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${S}/lib \
    -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=${S}/lib \
    -DXRT_INCLUDE_DIR=${RECIPE_SYSROOT}/usr/include/xrt \
    -DXRT_LIBRARIES=${RECIPE_SYSROOT}/usr/lib/libxilinxopencl.so \
    -DXRT_LIBRARIES=${RECIPE_SYSROOT}/usr/lib/libxrt_core.so \
    -DXRT_LIBRARIES=${RECIPE_SYSROOT}/usr/lib/libxrt_coreutil.so \
"

# >>> 关键：do_install 确保所有 .so 被复制到 ${D}${libdir}
do_install() {
    install -d ${D}${bindir}
    for exe in ${S}/Examples/*/*; do
        if [ -x "$exe" ]; then
            install -m 755 $exe ${D}${bindir}/ || true
        fi
    done

    install -d ${D}${datadir}/orb-slam3
    if [ -d "${S}/Vocabulary" ]; then
        cp -r ${S}/Vocabulary ${D}${datadir}/orb-slam3/ || true
    fi

    install -d ${D}${libdir}

    # 安装主库（ORB_SLAM3）
    if [ -f ${S}/lib/libORB_SLAM3.so ]; then
        install -m 755 ${S}/lib/libORB_SLAM3.so ${D}${libdir}/
    fi

    # 尝试多个可能位置来安装 DBoW2（防止源码中输出路径不同）
    if [ -f ${S}/lib/libDBoW2.so ]; then
        install -m 755 ${S}/lib/libDBoW2.so ${D}${libdir}/
    elif [ -f ${S}/ORB_SLAM3/lib/libDBoW2.so ]; then
        install -m 755 ${S}/ORB_SLAM3/lib/libDBoW2.so ${D}${libdir}/
    elif [ -f ${S}/Thirdparty/DBoW2/lib/libDBoW2.so ]; then
        install -m 755 ${S}/Thirdparty/DBoW2/lib/libDBoW2.so ${D}${libdir}/
    fi

    # g2o 可能来自 sysroot 或 thirdparty，优先取 thirdparty 编译产物
    if [ -f ${S}/lib/libg2o.so ]; then
        install -m 755 ${S}/lib/libg2o.so ${D}${libdir}/
    elif [ -f ${S}/Thirdparty/g2o/lib/libg2o.so ]; then
        install -m 755 ${S}/Thirdparty/g2o/lib/libg2o.so ${D}${libdir}/
    fi
}
# <<< do_install 结束

# 生成库包
PACKAGES =+ "${PN}-libs"

# 库文件归入 -libs 包（包含 thirdparty）
FILES_${PN}-libs = "\
    ${libdir}/libORB_SLAM3.so* \
    ${libdir}/libDBoW2.so* \
    ${libdir}/libg2o.so* \
"

# 明确运行时依赖关系（全部小写）
RDEPENDS_${PN}-libs += "pangolin"
RDEPENDS_${PN} += "${PN}-libs"

# 其余文件归入主包
FILES_${PN} += "${datadir}/orb-slam3/* ${bindir}/*"

# 跳过部分QA检查（可选）
INSANE_SKIP_${PN} += "ldflags dev-deps"
INSANE_SKIP_${PN}-libs += "dev-deps file-rdeps"
