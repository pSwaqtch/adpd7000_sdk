/*!
 * @brief     BIOZ APIs Implementation
 * @copyright Copyright (c) 2021 - Analog Devices Inc. All Rights Reserved.
 */

/*!
 * @addtogroup adi_adpd7000_sdk
 * @{
 */

/*============= I N C L U D E S ============*/
#include "adi_adpd7000.h"
#include <complex.h>
#include <math.h>
   
/*============= D E F I N E S ==============*/
#define PI   3.1415926 

/*============= D A T A ====================*/
float bioz_r_Cal   = 2000.0;
float bioz_r_tia   = 2000.0;
float bioz_r_limit = 650.0;
adi_adpd7000_bioz_eda_mode_e eda_mode;
    
/*============= C O D E ====================*/
int32_t adi_adpd7000_bioz_enable_slot(adi_adpd7000_device_t *device, adi_adpd7000_bioz_slot_num_e num)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(num > API_ADPD7000_BIOZ_SLOT_All);
    
    err = adi_adpd7000_hal_bf_write(device, BF_BIOZ_TIMESLOT_EN_INFO, num);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_set_slot_mode(adi_adpd7000_device_t *device, adi_adpd7000_bioz_slot_mode_e mode)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(mode > API_ADPD7000_BIOZ_SLOT_LOW_FREQ);
    
    err = adi_adpd7000_hal_bf_write(device, BF_BIOZ_TIMESLOT_MODE_INFO, mode);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_get_slot_num(adi_adpd7000_device_t *device, uint8_t *slot_num)
{
    int32_t  err;
    uint16_t data;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    
    err = adi_adpd7000_hal_bf_read(device, BF_BIOZ_TIMESLOT_EN_INFO, &data);
    ADPD7000_ERROR_RETURN(err);
    *slot_num = data;
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_set_timeslot_offset(adi_adpd7000_device_t *device, uint8_t slot, uint16_t offset)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(slot >= 18);
    
    err = adi_adpd7000_hal_bf_write(device, ADPD7000_TIME_SLOT_SPAN * slot + BF_BIOZ_TIMESLOT_OFFSET_A_INFO, offset);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_wave_set_freq(adi_adpd7000_device_t *device, uint8_t slot, uint32_t freq)
{
    int32_t  err;
    uint64_t fcw;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(slot >= 18);
    
    /* fcw = 2^26* freq/32e6 */
    fcw = (uint64_t)67108864 * (uint64_t)freq;
    fcw = fcw / 32000000;
    
    err = adi_adpd7000_hal_bf_write(device, ADPD7000_TIME_SLOT_SPAN * slot + BF_BIOZ_SINEFCW_L_A_INFO, fcw & 0xffff);
    ADPD7000_ERROR_RETURN(err);
    err = adi_adpd7000_hal_bf_write(device, ADPD7000_TIME_SLOT_SPAN * slot + BF_BIOZ_SINEFCW_H_A_INFO, (fcw >> 16) & 0xf);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_wave_set_phase(adi_adpd7000_device_t *device, uint8_t slot, uint16_t phase)
{
    int32_t  err;
    uint64_t phase_offset;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(phase > 360);
    ADPD7000_INVALID_PARAM_RETURN(slot >= 18);
    
    /* phase_offset = phase/360*2^16 */
    phase_offset = (uint64_t)65536 * (uint64_t)phase;
    phase_offset = phase_offset / 360;
    
    err = adi_adpd7000_hal_bf_write(device, ADPD7000_TIME_SLOT_SPAN * slot + BF_BIOZ_SINE_PHASE_OFFSET_A_INFO, phase_offset & 0xffff);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_wave_set_offset(adi_adpd7000_device_t *device, uint8_t slot, uint16_t offset)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(slot >= 18);
    
    err = adi_adpd7000_hal_bf_write(device, ADPD7000_TIME_SLOT_SPAN * slot + BF_BIOZ_SINE_OFFSET_A_INFO, offset);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_wave_set_amp(adi_adpd7000_device_t *device, uint8_t slot, uint16_t amp)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(slot >= 18);
    
    err = adi_adpd7000_hal_bf_write(device, ADPD7000_TIME_SLOT_SPAN * slot + BF_BIOZ_SINEAMPLITUDE_A_INFO, amp);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_cfg_wave(adi_adpd7000_device_t *device, uint8_t slot, uint16_t amp, uint32_t freq, uint16_t phase, uint16_t offset)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(slot >= 18);
    
    err = adi_adpd7000_bioz_wave_set_amp(device, slot, amp);
    ADPD7000_ERROR_RETURN(err);
    err = adi_adpd7000_bioz_wave_set_freq(device, slot, freq);
    ADPD7000_ERROR_RETURN(err);
    err = adi_adpd7000_bioz_wave_set_phase(device, slot, phase);
    ADPD7000_ERROR_RETURN(err);
    err = adi_adpd7000_bioz_wave_set_offset(device, slot, offset);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_tia_set_gain(adi_adpd7000_device_t *device, uint8_t slot, adi_adpd7000_bioz_tia_gain_res_e res, uint8_t cap)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(slot >= 18);
    ADPD7000_INVALID_PARAM_RETURN(res > API_ADPD7000_BIOZ_TIA_GAIN_10K);
    ADPD7000_INVALID_PARAM_RETURN(cap > 15);
    
    err = adi_adpd7000_hal_bf_write(device, ADPD7000_TIME_SLOT_SPAN * slot + BF_BIOZ_TIA_RGAIN_A_INFO, res);
    ADPD7000_ERROR_RETURN(err);
    err = adi_adpd7000_hal_bf_write(device, ADPD7000_TIME_SLOT_SPAN * slot + BF_BIOZ_TIA_CGAIN_A_INFO, cap);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_tia_enable_lowpower(adi_adpd7000_device_t *device, uint8_t slot, bool enable)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(slot >= 18);
    
    err = adi_adpd7000_hal_bf_write(device, ADPD7000_TIME_SLOT_SPAN * slot + BF_BIOZ_TIA_LPMODE_A_INFO, enable ? 1 : 0);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_bypass_sinc3_filter(adi_adpd7000_device_t *device, uint8_t slot, bool en_bypass)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(slot >= 18);
    
    err = adi_adpd7000_hal_bf_write(device, ADPD7000_TIME_SLOT_SPAN * slot + BF_BIOZ_SINC3BYP_A_INFO, en_bypass ? 1 : 0);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_sinc3_set_sample_rate(adi_adpd7000_device_t *device, uint8_t slot, adi_adpd7000_bioz_sinc3_os_rate_e rate)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(slot >= 18);
    ADPD7000_INVALID_PARAM_RETURN(rate > API_ADPD7000_BIOZ_SINC3_OVERSAMPLE_RATE_5);
    
    err = adi_adpd7000_hal_bf_write(device, ADPD7000_TIME_SLOT_SPAN * slot + BF_BIOZ_SINC3BYP_A_INFO, rate);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_enable_average(adi_adpd7000_device_t *device, uint8_t slot, bool enable)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(slot >= 18);
    
    err = adi_adpd7000_hal_bf_write(device, ADPD7000_TIME_SLOT_SPAN * slot + BF_BIOZ_AVRGEN_A_INFO, enable ? 1 : 0);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_set_average_sample(adi_adpd7000_device_t *device, uint8_t slot, adi_adpd7000_bioz_sinc3_avg_e sample)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(slot >= 18);
    ADPD7000_INVALID_PARAM_RETURN(sample > API_ADPD7000_BIOZ_SINC3_AVERAGE_16);
    
    err = adi_adpd7000_hal_bf_write(device, ADPD7000_TIME_SLOT_SPAN * slot + BF_BIOZ_AVRGNUM_A_INFO, sample);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_dft_set_phase(adi_adpd7000_device_t *device, uint8_t slot, uint16_t phase)
{
    int32_t  err;
    uint64_t phase_offset;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(phase > 360);
    ADPD7000_INVALID_PARAM_RETURN(slot >= 18);
    
    /* phase_offset = phase/360*2^16 */
    phase_offset = (uint64_t)65536 * (uint64_t)phase;
    phase_offset = phase_offset / 360;
    
    err = adi_adpd7000_hal_bf_write(device, ADPD7000_TIME_SLOT_SPAN * slot + BF_BIOZ_DFT_PHASE_OFFSET_A_INFO, phase_offset & 0xffff);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_dft_set_point_number(adi_adpd7000_device_t *device, uint8_t slot, adi_adpd7000_bioz_dft_point_e num)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(num > API_ADPD7000_BIOZ_DFT_POINT_8192);
    ADPD7000_INVALID_PARAM_RETURN(slot >= 18);
    
    err = adi_adpd7000_hal_bf_write(device, ADPD7000_TIME_SLOT_SPAN * slot + BF_BIOZ_DFTNUM_A_INFO, num);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_dft_enable_hanning(adi_adpd7000_device_t *device, uint8_t slot, bool enable)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(slot >= 18);
    
    err = adi_adpd7000_hal_bf_write(device, ADPD7000_TIME_SLOT_SPAN * slot + BF_BIOZ_HANNINGEN_A_INFO, enable ? 1 : 0);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_enable_tia(adi_adpd7000_device_t *device, uint8_t slot, bool enable)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(slot >= 18);
    
    err = adi_adpd7000_hal_bf_write(device, ADPD7000_TIME_SLOT_SPAN * slot + BF_BIOZ_TIAEN_A_INFO, enable ? 1 : 0);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_enable_exbuf(adi_adpd7000_device_t *device, uint8_t slot, bool enable)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(slot >= 18);
    
    err = adi_adpd7000_hal_bf_write(device, ADPD7000_TIME_SLOT_SPAN * slot + BF_BIOZ_EXBUFEN_A_INFO, enable ? 1 : 0);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_set_slot_connection(adi_adpd7000_device_t *device, uint8_t slot, adi_adpd7000_bioz_slot_connect_e mode)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_INVALID_PARAM_RETURN(mode > API_ADPD7000_BIOZ_CONN_BIO_IMP_CUR);
    
    switch (mode)
    {
    case API_ADPD7000_BIOZ_CONN_EXT_RCAL_VOL:
        err = adi_adpd7000_hal_reg_write(device, ADPD7000_TIME_SLOT_SPAN * slot + REG_BIOZ_ANA_CTRL2_A_ADDR, 0x2803);
        ADPD7000_ERROR_RETURN(err);
        err = adi_adpd7000_hal_reg_write(device, ADPD7000_TIME_SLOT_SPAN * slot + REG_BIOZ_ANA_CTRL3_A_ADDR, 0);
        ADPD7000_ERROR_RETURN(err);
        break;
    case API_ADPD7000_BIOZ_CONN_EXT_RCAL_CUR:
        err = adi_adpd7000_hal_reg_write(device, ADPD7000_TIME_SLOT_SPAN * slot + REG_BIOZ_ANA_CTRL2_A_ADDR, 0x0003);
        ADPD7000_ERROR_RETURN(err);
        err = adi_adpd7000_hal_reg_write(device, ADPD7000_TIME_SLOT_SPAN * slot + REG_BIOZ_ANA_CTRL3_A_ADDR, 0);
        ADPD7000_ERROR_RETURN(err);
        break;
    case API_ADPD7000_BIOZ_CONN_INT_RCAL_VOL:
        err = adi_adpd7000_hal_reg_write(device, ADPD7000_TIME_SLOT_SPAN * slot + REG_BIOZ_ANA_CTRL2_A_ADDR, 0x3c00);
        ADPD7000_ERROR_RETURN(err);
        err = adi_adpd7000_hal_reg_write(device, ADPD7000_TIME_SLOT_SPAN * slot + REG_BIOZ_ANA_CTRL3_A_ADDR, 0x03);
        ADPD7000_ERROR_RETURN(err);
        break;
    case API_ADPD7000_BIOZ_CONN_INT_RCAL_CUR:
        err = adi_adpd7000_hal_reg_write(device, ADPD7000_TIME_SLOT_SPAN * slot + REG_BIOZ_ANA_CTRL2_A_ADDR, 0x0000);
        ADPD7000_ERROR_RETURN(err);
        err = adi_adpd7000_hal_reg_write(device, ADPD7000_TIME_SLOT_SPAN * slot + REG_BIOZ_ANA_CTRL3_A_ADDR, 0x03);
        ADPD7000_ERROR_RETURN(err);
        break;
    case API_ADPD7000_BIOZ_CONN_BIO_IMP_VOL:
        err = adi_adpd7000_hal_reg_write(device, ADPD7000_TIME_SLOT_SPAN * slot + REG_BIOZ_ANA_CTRL2_A_ADDR, 0x5400);
        ADPD7000_ERROR_RETURN(err);
        err = adi_adpd7000_hal_reg_write(device, ADPD7000_TIME_SLOT_SPAN * slot + REG_BIOZ_ANA_CTRL3_A_ADDR, 0);
        ADPD7000_ERROR_RETURN(err);
        break;
    case API_ADPD7000_BIOZ_CONN_BIO_IMP_CUR:
        err = adi_adpd7000_hal_reg_write(device, ADPD7000_TIME_SLOT_SPAN * slot + REG_BIOZ_ANA_CTRL2_A_ADDR, 0x4000);
        ADPD7000_ERROR_RETURN(err);
        err = adi_adpd7000_hal_reg_write(device, ADPD7000_TIME_SLOT_SPAN * slot + REG_BIOZ_ANA_CTRL3_A_ADDR, 0);
        ADPD7000_ERROR_RETURN(err);
        break;
    default:
        break;
    }
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_sel_exbuf_input(adi_adpd7000_device_t *device, uint8_t slot, adi_adpd7000_bioz_imp_pin_e input)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    
    err = adi_adpd7000_hal_bf_write(device, ADPD7000_TIME_SLOT_SPAN * slot + BF_BIOZ_DSW_A_INFO, input);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_sel_tia_input(adi_adpd7000_device_t *device, uint8_t slot, adi_adpd7000_bioz_imp_pin_e input)
{
    int32_t  err;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    
    err = adi_adpd7000_hal_bf_write(device, ADPD7000_TIME_SLOT_SPAN * slot + BF_BIOZ_TSW_A_INFO, input);
    ADPD7000_ERROR_RETURN(err);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_read_fifo(adi_adpd7000_device_t *device, adi_adpd7000_fifo_config_t *fifo, uint32_t *bioz_real, uint32_t *bioz_imag, uint8_t *bioz_num)
{
    int32_t err;
    uint8_t i, bioz_count = 0;
    uint8_t fifo_data[3];
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();

    for (i = 0; i < fifo->bioz_slot; i++)
    {     
        err = adi_adpd7000_device_fifo_read_bytes(device, fifo_data, 3);
        ADPD7000_ERROR_RETURN(err);
        *bioz_real++ = (fifo_data[0] << 16) | (fifo_data[1] << 8) | (fifo_data[2]);
        
        err = adi_adpd7000_device_fifo_read_bytes(device, fifo_data, 3);
        ADPD7000_ERROR_RETURN(err);
        *bioz_imag++ = (fifo_data[0] << 16) | (fifo_data[1] << 8) | (fifo_data[2]);
        bioz_count++;
    }
    *bioz_num = bioz_count;
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_read_fifo_struct(adi_adpd7000_device_t *device, adi_adpd7000_fifo_config_t *fifo, adi_adpd7000_bioz_slot_data_t *bioz, uint8_t *bioz_num)
{
    int32_t err;  
    uint8_t i, bioz_count = 0;;
    uint8_t fifo_data[3];
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();

    for (i = 0; i < fifo->bioz_slot; i++)
    {     
        err = adi_adpd7000_device_fifo_read_bytes(device, fifo_data, 3);
        ADPD7000_ERROR_RETURN(err);
        bioz[i].real = (fifo_data[0] << 16) | (fifo_data[1] << 8) | (fifo_data[2]);
        
        err = adi_adpd7000_device_fifo_read_bytes(device, fifo_data, 3);
        ADPD7000_ERROR_RETURN(err);
        bioz[i].imag = (fifo_data[0] << 16) | (fifo_data[1] << 8) | (fifo_data[2]);
        bioz_count++;
    }
    *bioz_num = bioz_count;
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_cal_ms6_cali(uint32_t OOOOOOOO0O[6],uint32_t OO00O000OO[6],uint32_t OO000O0O0O[6],uint32_t O000000OOO[6],float*O0O0OO00O0,float*OO0000O0O0,float*OOO0OO00O0){int32_t O000OO0000[6];int32_t OO00OOO00O[6];int32_t O0O0O0O0OO[6];int32_t O00O0000OO[6];_Complex double OO00000000[6];int OO00OO0OOO=0;O0O0O0OO00:if(!(OO00OO0OOO<6))goto O0OOOO00O0;goto OO000O0O00;O0O0OO00OO:OO00OO0OOO++;goto O0O0O0OO00;OO000O0O00:{O000OO0000[OO00OO0OOO]=(((OOOOOOOO0O[OO00OO0OOO]&0x800000))>0)?(OOOOOOOO0O[OO00OO0OOO]-0xffffff-1):OOOOOOOO0O[OO00OO0OOO];OO00OOO00O[OO00OO0OOO]=(((OO00O000OO[OO00OO0OOO]&0x800000))>0)?(OO00O000OO[OO00OO0OOO]-0xffffff-1):OO00O000OO[OO00OO0OOO];O0O0O0O0OO[OO00OO0OOO]=(((OO000O0O0O[OO00OO0OOO]&0x800000))>0)?(OO000O0O0O[OO00OO0OOO]-0xffffff-1):OO000O0O0O[OO00OO0OOO];O00O0000OO[OO00OO0OOO]=(((O000000OOO[OO00OO0OOO]&0x800000))>0)?(O000000OOO[OO00OO0OOO]-0xffffff-1):O000000OOO[OO00OO0OOO];if(OO00OO0OOO==0){OO00000000[OO00OO0OOO]=O000OO0000[OO00OO0OOO]-OO00OOO00O[OO00OO0OOO]*I;}else{OO00000000[OO00OO0OOO]=O000OO0000[OO00OO0OOO]-O0O0O0O0OO[OO00OO0OOO]-(OO00OOO00O[OO00OO0OOO]-O00O0000OO[OO00OO0OOO])*I;}}goto O0O0OO00OO;O0OOOO00O0:;_Complex double OO0OOO000O=bioz_r_Cal*OO00000000[0];_Complex double O0000O0OOO=OO0OOO000O/OO00000000[1];_Complex double O0O0000000=OO0OOO000O/OO00000000[2];_Complex double OOOOOO00OO=OO0OOO000O/OO00000000[3];_Complex double O00OO0OO00=OO0OOO000O/OO00000000[4];_Complex double O0OO00O000=OO0OOO000O/OO00000000[5];_Complex double OO0O0000OO=-2*O0000O0OOO*O0O0000000*O0OO00O000*(O0000O0OOO*O0O0000000+O0000O0OOO*O0OO00O000-O0O0000000*O0OO00O000)/(O0000O0OOO*O0000O0OOO*O0O0000000*O0O0000000-2*O0000O0OOO*O0000O0OOO*O0O0000000*O0OO00O000+O0000O0OOO*O0000O0OOO*O0OO00O000*O0OO00O000-2*O0000O0OOO*O0O0000000*O0O0000000*O0OO00O000-2*O0000O0OOO*O0O0000000*O0OO00O000*O0OO00O000+O0O0000000*O0O0000000*O0OO00O000*O0OO00O000);_Complex double O0OOO0OO00=-2*O0000O0OOO*O0O0000000*O0OO00O000*(O0000O0OOO*O0O0000000-O0000O0OOO*O0OO00O000+O0O0000000*O0OO00O000)/(O0000O0OOO*O0000O0OOO*O0O0000000*O0O0000000-2*O0000O0OOO*O0000O0OOO*O0O0000000*O0OO00O000+O0000O0OOO*O0000O0OOO*O0OO00O000*O0OO00O000-2*O0000O0OOO*O0O0000000*O0O0000000*O0OO00O000-2*O0000O0OOO*O0O0000000*O0OO00O000*O0OO00O000+O0O0000000*O0O0000000*O0OO00O000*O0OO00O000);_Complex double O0000OOO00=-2*OOOOOO00OO*O00OO0OO00*O0OO00O000*(OOOOOO00OO*O00OO0OO00+OOOOOO00OO*O0OO00O000-O00OO0OO00*O0OO00O000)/(OOOOOO00OO*OOOOOO00OO*O00OO0OO00*O00OO0OO00-2*OOOOOO00OO*OOOOOO00OO*O00OO0OO00*O0OO00O000+OOOOOO00OO*OOOOOO00OO*O0OO00O000*O0OO00O000-2*OOOOOO00OO*O00OO0OO00*O00OO0OO00*O0OO00O000-2*OOOOOO00OO*O00OO0OO00*O0OO00O000*O0OO00O000+O00OO0OO00*O00OO0OO00*O0OO00O000*O0OO00O000);_Complex double OO0O000O00=-2*OOOOOO00OO*O00OO0OO00*O0OO00O000*(OOOOOO00OO*O00OO0OO00-OOOOOO00OO*O0OO00O000+O00OO0OO00*O0OO00O000)/(OOOOOO00OO*OOOOOO00OO*O00OO0OO00*O00OO0OO00-2*OOOOOO00OO*OOOOOO00OO*O00OO0OO00*O0OO00O000+OOOOOO00OO*OOOOOO00OO*O0OO00O000*O0OO00O000-2*OOOOOO00OO*O00OO0OO00*O00OO0OO00*O0OO00O000-2*OOOOOO00OO*O00OO0OO00*O0OO00O000*O0OO00O000+O00OO0OO00*O00OO0OO00*O0OO00O000*O0OO00O000);_Complex double O0000OO00O=(-OO0O0000OO*O0OOO0OO00*O0000OOO00-OO0O0000OO*O0OOO0OO00*OO0O000O00-OO0O0000OO*O0000OOO00*OO0O000O00+OO0O0000OO*O0000OOO00*O0OO00O000+OO0O0000OO*OO0O000O00*O0OO00O000-O0OOO0OO00*O0000OOO00*OO0O000O00+O0OOO0OO00*O0000OOO00*O0OO00O000+O0OOO0OO00*OO0O000O00*O0OO00O000)/(OO0O0000OO*O0000OOO00+OO0O0000OO*OO0O000O00+O0OOO0OO00*O0000OOO00+O0OOO0OO00*OO0O000O00);*O0O0OO00O0=cabs(O0000OO00O);*OO0000O0O0=catan(cimag(O0000OO00O)/creal(O0000OO00O))*180/PI;if(OOO0OO00O0!=NULL){OOO0OO00O0[0]=cabs(OO0O0000OO);OOO0OO00O0[1]=cabs(O0OOO0OO00);OOO0OO00O0[2]=cabs(O0000OOO00);OOO0OO00O0[3]=cabs(OO0O000O00);}return 0;}
int32_t adi_adpd7000_bioz_cal_ms6(uint32_t OOO0OO00OO[6],uint32_t O000000O0O[6],float*O0000OOO0O,float*O0O0000000,float*O0OO00O000){int32_t O0O0O000OO[6];int32_t O000OOOO0O[6];int OO0OO00OOO=0;OOOO0OOO0O:if(!(OO0OO00OOO<6))goto O0OOOOOO0O;goto OOO0O00OOO;O0OOOOO000:OO0OO00OOO++;goto OOOO0OOO0O;OOO0O00OOO:{O0O0O000OO[OO0OO00OOO]=(((OOO0OO00OO[OO0OO00OOO]&0x800000))>0)?(OOO0OO00OO[OO0OO00OOO]-0xffffff-1):OOO0OO00OO[OO0OO00OOO];O000OOOO0O[OO0OO00OOO]=(((O000000O0O[OO0OO00OOO]&0x800000))>0)?(O000000O0O[OO0OO00OOO]-0xffffff-1):O000000O0O[OO0OO00OOO];}goto O0OOOOO000;O0OOOOOO0O:;_Complex double OO00OO00O0=O0O0O000OO[0]-O000OOOO0O[0]*I;_Complex double O000OO0O00=O0O0O000OO[1]-O000OOOO0O[1]*I;_Complex double OO0O0O0OO0=O0O0O000OO[2]-O000OOOO0O[2]*I;_Complex double OO0OOOOOOO=O0O0O000OO[3]-O000OOOO0O[3]*I;_Complex double OO00000000=O0O0O000OO[4]-O000OOOO0O[4]*I;_Complex double O0000O0OO0=O0O0O000OO[5]-O000OOOO0O[5]*I;_Complex double O000O0O0O0=bioz_r_Cal*OO00OO00O0;_Complex double OOO0O0O000=O000O0O0O0/O000OO0O00;_Complex double OOOO0O0000=O000O0O0O0/OO0O0O0OO0;_Complex double O000O0OO00=O000O0O0O0/OO0OOOOOOO;_Complex double O00OOO0OOO=O000O0O0O0/OO00000000;_Complex double O0OO00OO0O=O000O0O0O0/O0000O0OO0;_Complex double O0O0O00O00=-2*OOO0O0O000*OOOO0O0000*O0OO00OO0O*(OOO0O0O000*OOOO0O0000+OOO0O0O000*O0OO00OO0O-OOOO0O0000*O0OO00OO0O)/(OOO0O0O000*OOO0O0O000*OOOO0O0000*OOOO0O0000-2*OOO0O0O000*OOO0O0O000*OOOO0O0000*O0OO00OO0O+OOO0O0O000*OOO0O0O000*O0OO00OO0O*O0OO00OO0O-2*OOO0O0O000*OOOO0O0000*OOOO0O0000*O0OO00OO0O-2*OOO0O0O000*OOOO0O0000*O0OO00OO0O*O0OO00OO0O+OOOO0O0000*OOOO0O0000*O0OO00OO0O*O0OO00OO0O);_Complex double O0O00O00OO=-2*OOO0O0O000*OOOO0O0000*O0OO00OO0O*(OOO0O0O000*OOOO0O0000-OOO0O0O000*O0OO00OO0O+OOOO0O0000*O0OO00OO0O)/(OOO0O0O000*OOO0O0O000*OOOO0O0000*OOOO0O0000-2*OOO0O0O000*OOO0O0O000*OOOO0O0000*O0OO00OO0O+OOO0O0O000*OOO0O0O000*O0OO00OO0O*O0OO00OO0O-2*OOO0O0O000*OOOO0O0000*OOOO0O0000*O0OO00OO0O-2*OOO0O0O000*OOOO0O0000*O0OO00OO0O*O0OO00OO0O+OOOO0O0000*OOOO0O0000*O0OO00OO0O*O0OO00OO0O);_Complex double OO0OO0OOOO=-2*O000O0OO00*O00OOO0OOO*O0OO00OO0O*(O000O0OO00*O00OOO0OOO+O000O0OO00*O0OO00OO0O-O00OOO0OOO*O0OO00OO0O)/(O000O0OO00*O000O0OO00*O00OOO0OOO*O00OOO0OOO-2*O000O0OO00*O000O0OO00*O00OOO0OOO*O0OO00OO0O+O000O0OO00*O000O0OO00*O0OO00OO0O*O0OO00OO0O-2*O000O0OO00*O00OOO0OOO*O00OOO0OOO*O0OO00OO0O-2*O000O0OO00*O00OOO0OOO*O0OO00OO0O*O0OO00OO0O+O00OOO0OOO*O00OOO0OOO*O0OO00OO0O*O0OO00OO0O);_Complex double O00000OOO0=-2*O000O0OO00*O00OOO0OOO*O0OO00OO0O*(O000O0OO00*O00OOO0OOO-O000O0OO00*O0OO00OO0O+O00OOO0OOO*O0OO00OO0O)/(O000O0OO00*O000O0OO00*O00OOO0OOO*O00OOO0OOO-2*O000O0OO00*O000O0OO00*O00OOO0OOO*O0OO00OO0O+O000O0OO00*O000O0OO00*O0OO00OO0O*O0OO00OO0O-2*O000O0OO00*O00OOO0OOO*O00OOO0OOO*O0OO00OO0O-2*O000O0OO00*O00OOO0OOO*O0OO00OO0O*O0OO00OO0O+O00OOO0OOO*O00OOO0OOO*O0OO00OO0O*O0OO00OO0O);_Complex double O0O0OO0OO0=(-O0O0O00O00*O0O00O00OO*OO0OO0OOOO-O0O0O00O00*O0O00O00OO*O00000OOO0-O0O0O00O00*OO0OO0OOOO*O00000OOO0+O0O0O00O00*OO0OO0OOOO*O0OO00OO0O+O0O0O00O00*O00000OOO0*O0OO00OO0O-O0O00O00OO*OO0OO0OOOO*O00000OOO0+O0O00O00OO*OO0OO0OOOO*O0OO00OO0O+O0O00O00OO*O00000OOO0*O0OO00OO0O)/(O0O0O00O00*OO0OO0OOOO+O0O0O00O00*O00000OOO0+O0O00O00OO*OO0OO0OOOO+O0O00O00OO*O00000OOO0);*O0000OOO0O=cabs(O0O0OO0OO0);*O0O0000000=catan(cimag(O0O0OO0OO0)/creal(O0O0OO0OO0))*180/PI;if(O0OO00O000!=NULL){O0OO00O000[0]=cabs(O0O0O00O00);O0OO00O000[1]=cabs(O0O00O00OO);O0OO00O000[2]=cabs(OO0OO0OOOO);O0OO00O000[3]=cabs(O00000OOO0);}return 0;}
int32_t adi_adpd7000_bioz_cal_ms6_open_contact_cap(uint32_t OOO0O0000O[6],uint32_t O000OO0OO0[6],uint32_t O0O0O000O0,float O000000000[4]){int OOO00O0000[6];int OOO0OOO0O0[6];float O00000O0O0;int OOO0O0OO0O=0;O0O0OO0OO0:if(!(OOO0O0OO0O<6))goto O00OO000OO;goto OOO000O00O;O0OOO00O0O:OOO0O0OO0O++;goto O0O0OO0OO0;OOO000O00O:{OOO00O0000[OOO0O0OO0O]=(((OOO0O0000O[OOO0O0OO0O]&0x800000))>0)?(OOO0O0000O[OOO0O0OO0O]-0xffffff-1):OOO0O0000O[OOO0O0OO0O];OOO0OOO0O0[OOO0O0OO0O]=(((O000OO0OO0[OOO0O0OO0O]&0x800000))>0)?(O000OO0OO0[OOO0O0OO0O]-0xffffff-1):O000OO0OO0[OOO0O0OO0O];}goto O0OOO00O0O;O00OO000OO:;_Complex double OO0OOO0O00=OOO00O0000[0]-OOO0OOO0O0[0]*I;_Complex double OO0OO0OOOO=OOO00O0000[1]-OOO0OOO0O0[1]*I;_Complex double O0OOO0O000=OOO00O0000[2]-OOO0OOO0O0[2]*I;_Complex double O00OOOOO00=OOO00O0000[3]-OOO0OOO0O0[3]*I;_Complex double OOO00OO0O0=OOO00O0000[4]-OOO0OOO0O0[4]*I;_Complex double OO0O000O0O=OOO00O0000[5]-OOO0OOO0O0[5]*I;_Complex double OO00O00O00=bioz_r_Cal*(OO0OOO0O00-OO0OO0OOOO)*(OO0OOO0O00-O0OOO0O000)/OO0OOO0O00/(O0OOO0O000-OO0OO0OOOO);_Complex double O0OO00000O=bioz_r_Cal*(OO0OOO0O00-OO0OO0OOOO)*(OO0OOO0O00-O00OOOOO00)/OO0OOO0O00/(O00OOOOO00-OO0OO0OOOO);_Complex double OO0OO00O0O=bioz_r_Cal*(OO0OOO0O00-OO0OO0OOOO)*(OO0OOO0O00-OOO00OO0O0)/(OOO00OO0O0-OO0OO0OOOO)/OO0OOO0O00;_Complex double OO00OO00O0=bioz_r_Cal*(OO0OOO0O00-OO0OO0OOOO)*(OO0OOO0O00-OO0O000O0O)/(OO0O000O0O-OO0OO0OOOO)/OO0OOO0O00;O00000O0O0=-1000000000000.0/(2*PI*O0O0O000O0*cimag(OO00O00O00));O000000000[0]=O00000O0O0;O00000O0O0=-1000000000000.0/(2*PI*O0O0O000O0*cimag(O0OO00000O));O000000000[1]=O00000O0O0;O00000O0O0=-1000000000000.0/(2*PI*O0O0O000O0*cimag(OO0OO00O0O));O000000000[2]=O00000O0O0;O00000O0O0=-1000000000000.0/(2*PI*O0O0O000O0*cimag(OO00OO00O0));O000000000[3]=O00000O0O0;return 0;}

int32_t adi_adpd7000_bioz_cal_ms4(uint32_t data_i[4], uint32_t data_q[4], float *amp, float *phase)
{
    float real, imag;
    float mod[4],ph[4];
    float zbody, ph_cal;
    uint8_t i;
    
    for (i = 0; i < 4; i++)
    {
        real = (((data_i[i] & 0x800000)) > 0) ? ((float)data_i[i] - (float)0xffffff - 1) : (float)data_i[i];
        imag = (((data_q[i] & 0x800000)) > 0) ? ((float)data_q[i] - (float)0xffffff - 1) : (float)data_q[i];
        mod[i] = sqrt(real * real + imag * imag);
        ph[i] = atan(-imag/real);
    }
    zbody = bioz_r_Cal * mod[1] * mod[2] / (mod[0] * mod[3]);
    ph_cal = (ph[1] + ph[2] - ph[3] - ph[0]) * 180.0 / PI;
    *amp = zbody;
    *phase = ph_cal;
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_cal_ms2(uint32_t data_i[2], uint32_t data_q[2], float *amp)
{
    int32_t real_data[2];
    int32_t imag_data[2];
    uint8_t i;
    float rbody;
    _Complex double M0, M1;
    
    for (i = 0; i < 2; i++)
    {
        real_data[i] = (((data_i[i] & 0x800000)) > 0) ? (data_i[i] - 0xffffff - 1) : data_i[i];
        imag_data[i] = (((data_q[i] & 0x800000)) > 0) ? (data_q[i] - 0xffffff - 1) : data_q[i];
    }
    
    if (eda_mode == API_ADPD7000_BIOZ_EDA_MODE_ACV)
    {
        M0 = real_data[0] - imag_data[0] * I;
        M1 = real_data[1] - imag_data[1] * I;
        rbody = bioz_r_tia * cabs(M0) / cabs(M1);
    }
    else if ((eda_mode == API_ADPD7000_BIOZ_EDA_MODE_DCV) || (eda_mode == API_ADPD7000_BIOZ_EDA_MODE_DCI))
    {
      float real0 = fabs((float)real_data[0]);
      float imag0 = fabs((float)imag_data[0]);
      float real1 = fabs((float)real_data[1]);
      float imag1 = fabs((float)imag_data[1]);
      rbody = bioz_r_tia * (real0 / imag0 + real1 / imag1) / 2;
    }
    *amp = rbody;
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_set_rcal(float rcal)
{
    bioz_r_Cal = rcal;
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_get_rcal(float *rcal)
{
    if (rcal == NULL)
    {
        return -1;
    }
    *rcal = bioz_r_Cal;
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_update_rtia(adi_adpd7000_device_t *device)
{
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    uint32_t r_tia_lookup[14] = {1000, 2000, 3000, 4000, 6000, 8000, 10000, 15000, 30000, 60000, 125000, 250000, 500000, 1000000};
    int32_t err;
    uint16_t res;
    
    err = adi_adpd7000_hal_bf_read(device, BF_BIOZ_TIA_RGAIN_A_INFO, &res);
    ADPD7000_ERROR_RETURN(err);
    
    res = (res > 13) ? 13 : res;
    
    bioz_r_tia = (res <= 6) ? (1.0493 * (float)r_tia_lookup[res]) : (1.1162 * (float)r_tia_lookup[res]);
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_update_rlimit(adi_adpd7000_device_t *device)
{
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    uint32_t r_limit_lookup[4] = {0, 650, 1300, 0};
    int32_t err;
    uint16_t res;
    
    err = adi_adpd7000_hal_bf_read(device, BF_BIOZ_CURRENT_LIMIT_B_INFO, &res);
    ADPD7000_ERROR_RETURN(err);
    
    if ((res == 1) || (res == 2))
    {
        bioz_r_limit = r_limit_lookup[res];
    }
    else
    {
        return API_ADPD7000_ERROR_INVALID_PARAM;
    }
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_update_eda_mode(adi_adpd7000_device_t *device)
{
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    int32_t err;
    uint16_t dclo_l_en, dclo_m_en, dclo_h_en;
    uint16_t sinefcw_h, sinefcw_l;
    
    err = adi_adpd7000_hal_bf_read(device, BF_BIOZ_DCLO_L_EN_A_INFO, &dclo_l_en);
    ADPD7000_ERROR_RETURN(err);
    err = adi_adpd7000_hal_bf_read(device, BF_BIOZ_DCLO_M_EN_A_INFO, &dclo_m_en);
    ADPD7000_ERROR_RETURN(err);  
    err = adi_adpd7000_hal_bf_read(device, BF_BIOZ_DCLO_H_EN_A_INFO, &dclo_h_en);
    ADPD7000_ERROR_RETURN(err);
    err = adi_adpd7000_hal_bf_read(device, BF_BIOZ_SINEFCW_H_A_INFO, &sinefcw_h);
    ADPD7000_ERROR_RETURN(err);
    err = adi_adpd7000_hal_bf_read(device, BF_BIOZ_SINEFCW_L_A_INFO, &sinefcw_l);
    ADPD7000_ERROR_RETURN(err);
    
    if (dclo_l_en | dclo_m_en | dclo_h_en)
    {
        eda_mode = API_ADPD7000_BIOZ_EDA_MODE_DCI;
    }
    else if ((sinefcw_h + sinefcw_l) > 0)
    {
        eda_mode = API_ADPD7000_BIOZ_EDA_MODE_ACV;
    }
    else
    {
        eda_mode = API_ADPD7000_BIOZ_EDA_MODE_DCV;
    }
    
    return API_ADPD7000_ERROR_OK;
}

int32_t adi_adpd7000_bioz_get_internal_rcal(adi_adpd7000_device_t *device, float *rcal)
{
    int32_t  err = API_ADPD7000_ERROR_OK;
    uint16_t fuse = 0;
    uint16_t timeout = 0x1000;
    uint16_t data, fuse_code;
    ADPD7000_NULL_POINTER_RETURN(device);
    ADPD7000_LOG_FUNC();
    ADPD7000_NULL_POINTER_RETURN(rcal);
    
    err = adi_adpd7000_device_enable_slot_operation_mode_go(device, false);
    ADPD7000_ERROR_RETURN(err);
    err = adi_adpd7000_hal_bf_write(device, 0x00000053, 0x00000104, 1);
    ADPD7000_ERROR_RETURN(err);
    err = adi_adpd7000_hal_reg_write(device, 0x00000044, 7);
    ADPD7000_ERROR_RETURN(err);
    
    while (timeout--)
    {
        err = adi_adpd7000_hal_bf_read(device, 0x000000d6, 0x00000201, &fuse);
        ADPD7000_ERROR_RETURN(err);
        if (fuse == 2)
        {
            break;
        }
    }
    
    if (fuse == 2)
    {
        err = adi_adpd7000_hal_reg_read(device, 0x000000c1, &data);
        ADPD7000_ERROR_RETURN(err);
        fuse_code = data << 8;
        err = adi_adpd7000_hal_reg_read(device, 0x000000c2, &data);
        ADPD7000_ERROR_RETURN(err);
        fuse_code |= data;
        *rcal = (((float)fuse_code - 1024) / 1000.0 + 1) * 2000.0;
    }
    else
    {
        err = API_ADPD7000_ERROR_FUSE_NOT_DONE;
    }

    adi_adpd7000_hal_reg_write(device, 0x00000044, 0);
    adi_adpd7000_hal_bf_write(device, 0x00000053, 0x00000104, 0);
    
    return err;
}
/*! @} */
