LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)
 
LOCAL_MODULE:= libNS
LOCAL_CFLAGS = -DFIXED_POINT -DUSE_KISS_FFT -DEXPORT="" -UHAVE_CONFIG_H
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_LDLIBS := -llog
LOCAL_SRC_FILES :=\
./speech_NS.cpp\
./webrtc_ns/complex_bit_reverse.c\
./webrtc_ns/complex_fft.c\
./webrtc_ns/copy_set_operations.c\
./webrtc_ns/cross_correlation.c\
./webrtc_ns/division_operations.c\
./webrtc_ns/downsample_fast.c\
./webrtc_ns/energy.c\
./webrtc_ns/fft4g.c\
./webrtc_ns/get_scaling_square.c\
./webrtc_ns/min_max_operations.c\
./webrtc_ns/noise_suppression_x.c\
./webrtc_ns/noise_suppression.c\
./webrtc_ns/ns_core.c\
./webrtc_ns/nsx_core_c.c\
./webrtc_ns/nsx_core_neon_offsets.c\
./webrtc_ns/nsx_core.c\
./webrtc_ns/real_fft.c\
./webrtc_ns/resample_48khz.c\
./webrtc_ns/resample_by_2_internal.c\
./webrtc_ns/resample_by_2_mips.c\
./webrtc_ns/resample_by_2.c\
./webrtc_ns/resample_fractional.c\
./webrtc_ns/resample.c\
./webrtc_ns/ring_buffer.c\
./webrtc_ns/spl_init.c\
./webrtc_ns/spl_sqrt_floor.c\
./webrtc_ns/spl_sqrt.c\
./webrtc_ns/splitting_filter.c\
./webrtc_ns/vector_scaling_operations.c
 
 
include $(BUILD_SHARED_LIBRARY)