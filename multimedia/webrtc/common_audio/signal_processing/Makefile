include $(ROOT)/config.mk

LOCAL_MODULE := libwebrtc_spl.a
LOCAL_SRC_FILES := \
    auto_corr_to_refl_coef.c \
    auto_correlation.c \
    complex_fft.c \
    copy_set_operations.c \
    cross_correlation.c \
    division_operations.c \
    dot_product_with_scale.c \
    downsample_fast.c \
    energy.c \
    filter_ar.c \
    filter_ma_fast_q12.c \
    get_hanning_window.c \
    get_scaling_square.c \
    ilbc_specific_functions.c \
    levinson_durbin.c \
    lpc_to_refl_coef.c \
    min_max_operations.c \
    randomization_functions.c \
    real_fft.c \
    refl_coef_to_lpc.c \
    resample.c \
    resample_48khz.c \
    resample_by_2.c \
    resample_by_2_internal.c \
    resample_fractional.c \
    spl_init.c \
    spl_sqrt.c \
    spl_version.c \
    splitting_filter.c \
    sqrt_of_one_minus_x_squared.c \
    vector_scaling_operations.c

ifeq ($(ARCH_ARM_HAVE_ARMV7A),true)
LOCAL_SRC_FILES += \
    filter_ar_fast_q12_armv7.S
else
LOCAL_SRC_FILES += \
    filter_ar_fast_q12.c
endif

ifeq ($(TARGET_ARCH),arm)
LOCAL_SRC_FILES += \
    complex_bit_reverse_arm.S \
    spl_sqrt_floor_arm.S
else
LOCAL_SRC_FILES += \
    complex_bit_reverse.c \
    spl_sqrt_floor.c
endif


include $(ROOT)/build_library.mk

