/*!
 * @brief     ADPD7000 Device Level APIs Implementation
 * @copyright Copyright (c) 2021 - Analog Devices Inc. All Rights Reserved.
 */

/*!
 * @addtogroup adi_adpd7000_sdk
 * @{
 */

/*============= I N C L U D E S ============*/
#include "adi_adpd7000.h"

/*============= D E F I N E S ==============*/

/*============= D A T A ====================*/

/*============= C O D E ====================*/
int32_t adi_adpd7000_device_get_id(adi_adpd7000_device_t *device, uint8_t *chip_id, uint8_t *chip_rev)
{
    int32_t  err;
    uint16_t id, rev;

    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_NULL_POINTER_RETURN(chip_id);
    ADPD7000_NULL_POINTER_RETURN(chip_rev);
    ADPD7000_LOG_FUNC();
    
    err = adi_adpd7000_hal_bf_read(device, BF_CHIP_ID_INFO, &id);
    ADPD7000_ERROR_RETURN(err);
    *chip_id = (uint8_t)id;

    err = adi_adpd7000_hal_bf_read(device, BF_VERSION_INFO, &rev);
    ADPD7000_ERROR_RETURN(err);
    *chip_rev = (uint8_t)rev;
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_device_get_sdk_rev(adi_adpd7000_device_t *device, uint32_t *rev)
{
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_NULL_POINTER_RETURN(rev);
    ADPD7000_LOG_FUNC();

    *rev = ADPD7000_SDK_VER;
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_device_sw_reset(adi_adpd7000_device_t *device)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    
    err = adi_adpd7000_hal_bf_write(device, BF_SW_RESET_INFO, 1);
    ADPD7000_ERROR_RETURN(err);
    err = adi_adpd7000_hal_bf_write(device, BF_SW_RESET_INFO, 0);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_device_init(adi_adpd7000_device_t *device)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    
    err = adi_adpd7000_device_sw_reset(device);
    ADPD7000_ERROR_RETURN(err);
    
    /* Change default trim */
    err = adi_adpd7000_hal_reg_write(device, 0x0046, 0x2004);
    ADPD7000_ERROR_RETURN(err);
    err = adi_adpd7000_hal_reg_write(device, 0x004c, 0x400b);
    ADPD7000_ERROR_RETURN(err);
    err = adi_adpd7000_hal_reg_write(device, 0x0074, 0x0028);
    ADPD7000_ERROR_RETURN(err);
    err = adi_adpd7000_hal_reg_write(device, 0x0077, 0x0100);
    ADPD7000_ERROR_RETURN(err);
        
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_device_enable_slot_operation_mode_go(adi_adpd7000_device_t *device, bool enable)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();

    err = adi_adpd7000_hal_bf_write(device, BF_OP_MODE_INFO, enable ? 1 : 0);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_device_set_fifo_threshold(adi_adpd7000_device_t *device, uint16_t threshold)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(threshold > 0x1ff);

    err = adi_adpd7000_hal_bf_write(device, BF_FIFO_TH_INFO, threshold);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_device_clr_fifo(adi_adpd7000_device_t *device)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();

    err = adi_adpd7000_hal_bf_write(device, BF_CLEAR_FIFO_INFO, 1);
    ADPD7000_ERROR_RETURN(err);
    err = adi_adpd7000_hal_bf_write(device, BF_CLEAR_FIFO_INFO, 0);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_device_get_fifo_count(adi_adpd7000_device_t *device, uint16_t *count)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_NULL_POINTER_RETURN(count);
    ADPD7000_LOG_FUNC();

    err = adi_adpd7000_hal_bf_read(device, BF_FIFO_BYTE_COUNT_INFO, count);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_device_fifo_read_bytes(adi_adpd7000_device_t *device, uint8_t *data, uint32_t len)
{
    int32_t err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_NULL_POINTER_RETURN(data);

    err = adi_adpd7000_hal_fifo_read_bytes(device, REG_FIFO_DATA_ADDR, data, len);
    ADPD7000_ERROR_RETURN(err);

    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_device_fifo_get_fifo_int_status(adi_adpd7000_device_t *device, uint8_t *status)
{
    int32_t err;
    uint16_t data;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_NULL_POINTER_RETURN(status);

    err = adi_adpd7000_hal_reg_read(device, REG_FIFO_STATUS_ADDR, &data);
    ADPD7000_ERROR_RETURN(err);
    *status = (data >> 12) & 0x07;

    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_device_enable_fifo_thres_interrupt(adi_adpd7000_device_t *device, adi_adpd7000_interrupt_type_e type, bool enable)
{
    int32_t err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    
    err = adi_adpd7000_hal_bf_write(device, type + BF_INTX_EN_FIFO_TH_INFO, enable ? 1 : 0);
    ADPD7000_ERROR_RETURN(err);

    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_device_enable_auto_clear_int(adi_adpd7000_device_t *device, bool enable)
{
    int32_t err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    
    err = adi_adpd7000_hal_bf_write(device, BF_INT_ACLEAR_FIFO_INFO, enable ? 1 : 0);
    ADPD7000_ERROR_RETURN(err);

    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_device_get_sequence_fifo_config(adi_adpd7000_device_t *device, adi_adpd7000_fifo_config_t *fifo)
{
    int32_t err;
    uint16_t data, i;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_NULL_POINTER_RETURN(fifo);
    
    err = adi_adpd7000_hal_bf_read(device, BF_ECG_TIMESLOT_EN_INFO, &data);
    ADPD7000_ERROR_RETURN(err);
    fifo->ecg_slot = data;
    err = adi_adpd7000_hal_bf_read(device, BF_PPG_TIMESLOT_EN_INFO, &data);
    ADPD7000_ERROR_RETURN(err);
    fifo->ppg_slot = data;
    err = adi_adpd7000_hal_bf_read(device, BF_BIOZ_TIMESLOT_EN_INFO, &data);
    ADPD7000_ERROR_RETURN(err);
    fifo->bioz_slot = data;
    
    err = adi_adpd7000_hal_bf_read(device, BF_ENA_STAT_ECG_INFO, &data);
    ADPD7000_ERROR_RETURN(err);
    fifo->ecg_size = (data == 1) ? 4 : 3;
    if ((fifo->ppg_slot == 0) && (fifo->bioz_slot == 0))
    {
        fifo->ecg_over_sample = 1;
    }
    else
    {
        err = adi_adpd7000_hal_bf_read(device, BF_ECG_OVERSAMPLING_RATIO_INFO, &data);
        ADPD7000_ERROR_RETURN(err);
        fifo->ecg_over_sample = data; 
    }
    fifo->sequence_size = fifo->ecg_slot * fifo->ecg_over_sample * fifo->ecg_size;
    
    fifo->ppg_chnl_num = 0;
    for (i = 0; i < fifo->ppg_slot; i++)
    {
        fifo->ppg_chnl_num += 1;
        err = adi_adpd7000_hal_bf_read(device, ADPD7000_TIME_SLOT_SPAN * i + BF_CHANNEL_EN_A_INFO, &data);
        ADPD7000_ERROR_RETURN(err);
        fifo->ppg_fifo[i].ppg_chl2_en = data;
        fifo->ppg_chnl_num += fifo->ppg_fifo[i].ppg_chl2_en;
      
        err = adi_adpd7000_hal_bf_read(device, ADPD7000_TIME_SLOT_SPAN * i + BF_SIGNAL_SIZE_A_INFO, &data);
        ADPD7000_ERROR_RETURN(err);
        fifo->ppg_fifo[i].signal_size = data;
        
        err = adi_adpd7000_hal_bf_read(device, ADPD7000_TIME_SLOT_SPAN * i + BF_DARK_SIZE_A_INFO, &data);
        ADPD7000_ERROR_RETURN(err);
        fifo->ppg_fifo[i].dark_size = data;
        
        err = adi_adpd7000_hal_bf_read(device, ADPD7000_TIME_SLOT_SPAN * i + BF_LIT_SIZE_A_INFO, &data);
        ADPD7000_ERROR_RETURN(err);
        fifo->ppg_fifo[i].lit_size = data;
        
        fifo->sequence_size += (fifo->ppg_fifo[i].signal_size + fifo->ppg_fifo[i].dark_size +fifo->ppg_fifo[i].lit_size) * (1 + fifo->ppg_fifo[i].ppg_chl2_en);
    }
        
    fifo->sequence_size += 6 * fifo->bioz_slot;

    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_device_enable_internal_osc_960k(adi_adpd7000_device_t *device)
{
    int32_t err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();

    err = adi_adpd7000_hal_bf_write(device, BF_OSC_960K_EN_INFO, 1);
    ADPD7000_ERROR_RETURN(err);
    err = adi_adpd7000_hal_bf_write(device, BF_ALT_CLOCKS_INFO, API_ADPD7000_SYS_CLOCK_INT_CLK);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_device_set_osc_trim(adi_adpd7000_device_t *device, uint16_t high_osc, uint16_t low_osc)
{
    int32_t err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();

    err = adi_adpd7000_hal_bf_write(device, BF_OSC_32M_FREQ_ADJ_INFO, high_osc); 
    ADPD7000_ERROR_RETURN(err);
    err = adi_adpd7000_hal_bf_write(device, BF_OSC_960K_FREQ_ADJ_INFO, low_osc);  
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_device_config_ext_clock(adi_adpd7000_device_t *device, adi_adpd7000_sys_clock_src_e src, uint8_t clk_pin)
{
    int32_t err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(src > API_ADPD7000_SYS_CLOCK_EXT_TM);
    ADPD7000_INVALID_PARAM_RETURN(clk_pin > 2);
    
    err = adi_adpd7000_hal_bf_write(device, BF_ALT_CLOCKS_INFO, src);
    ADPD7000_ERROR_RETURN(err);
    err = adi_adpd7000_hal_bf_write(device, BF_ALT_CLK_GPIO_INFO, clk_pin);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_device_set_slot_freq(adi_adpd7000_device_t *device, uint32_t sys_clk,  uint32_t freq)
{
    int32_t err;
    uint32_t data;
    ADPD7000_NULL_POINTER_RETURN(device);

    data = sys_clk / freq;
    err = adi_adpd7000_hal_bf_write(device, BF_TIMESLOT_PERIOD_L_INFO, data & 0xffff);
    ADPD7000_ERROR_RETURN(err);
    err = adi_adpd7000_hal_bf_write(device, BF_TIMESLOT_PERIOD_H_INFO, (data >> 16 ) & 0x7f);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_device_enbale_sleep_mode(adi_adpd7000_device_t *device, bool enable)
{
    int32_t err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();

    err = adi_adpd7000_hal_bf_write(device, BF_GO_SLEEP_INFO, enable ? 1 : 0);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_device_enable_losc_efuse(adi_adpd7000_device_t *device, bool enable)
{
    int32_t err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    
    err = adi_adpd7000_hal_bf_write(device, BF_OSC_960K_EFUSE_CTRL_INFO, enable ? 0 : 1);
    ADPD7000_ERROR_RETURN(err);

    return err;
}

int32_t adi_adpd7000_device_clr_fifo_int(adi_adpd7000_device_t *device)
{
    int32_t err;
    err = adi_adpd7000_hal_bf_write(device, BF_INT_FIFO_TH_INFO, 1);
    ADPD7000_ERROR_RETURN(err);
    return err;
}
/*! @} */
