#ifndef _CAM_ISP10_CTRL_ITF_IMC_H_
#define _CAM_ISP10_CTRL_ITF_IMC_H_

#include "CamIsp1xCtrItf.h"
#include "V4l2Isp10Ioctl.h"

#include <sys/poll.h>

using namespace std;

struct CamIsp10Config {
  struct cifisp_dpcc_config dpcc_config;
  struct cifisp_bls_config bls_config;
  struct cifisp_sdg_config sdg_config;
  struct cifisp_hst_config hst_config;
  struct cifisp_lsc_config lsc_config;
  struct cifisp_awb_gain_config awb_gain_config;
  struct cifisp_awb_meas_config awb_meas_config;
  struct cifisp_flt_config flt_config;
  struct cifisp_bdm_config bdm_config;
  struct cifisp_ctk_config ctk_config;
  struct cifisp_goc_config goc_config;
  struct cifisp_cproc_config cproc_config;
  struct cifisp_aec_config aec_config;
  struct cifisp_afc_config afc_config;
  struct cifisp_ie_config ie_config;
  struct cifisp_dpf_config dpf_config;
  struct cifisp_dpf_strength_config dpf_strength_config;
  bool_t enabled[HAL_ISP_MODULE_MAX_ID_ID + 1];
  int flt_denoise_level;
  int flt_sharp_level;
};

struct CamIsp10ConfigSet {
  unsigned int active_configs;
  struct CamIsp10Config configs;
  bool_t enabled[HAL_ISP_MODULE_MAX_ID_ID + 1];
};

class CamIsp10CtrItf: public CamIsp1xCtrItf {
 public:
  CamIsp10CtrItf(CamHwItf* camHwItf, int devFd);
  ~CamIsp10CtrItf();

  virtual bool init(const char* tuningFile,
                    const char* ispDev);
  virtual bool deInit();
  virtual void transDrvMetaDataToHal(const void* drvMeta, struct HAL_Buffer_MetaData* halMeta);
  virtual bool configureISP(const void* config);
  
  virtual bool getTimeAndGain(float* time, float* gain);
  virtual bool getMaxMinTime(float* max_time, float* min_time);
  virtual bool getMaxMinGain(float* max_gain, float* min_gain);
  virtual bool getMeanLuma(float* MeanLuma);
  virtual void setNightMode(bool night_mode);
  virtual void setDayNightSwitch(enum HAL_DAYNIGHT_MODE sw);
  virtual void getAeState(enum HAL_AE_STATE* ae_state);
  virtual bool getIspConfig(enum HAL_ISP_SUB_MODULE_ID_e mod_id,
      bool_t& enabled, CamIspCtrItf::Configuration& cfg);
  virtual int setExposure(unsigned int vts, unsigned int exposure, unsigned int gain, unsigned int gain_percent);
  virtual int setAutoAdjustFps(bool auto_adjust_fps);

 protected:
  virtual bool getSensorModedata
  (
      struct isp_supplemental_sensor_mode_data* drvCfg,
      CamIA10_SensorModeData* iaCfg
  );
  virtual bool applySubDevConfig(struct CamIA10_Results* results);
  virtual bool applyIspConfig(struct CamIsp10ConfigSet* isp_cfg);
  virtual bool convertIspStats(struct cifisp_stat_buffer* isp_stats,
                               struct CamIA10_Stats* ia_stats);
  virtual bool convertIAResults(
      struct CamIsp10ConfigSet* isp_cfg,
      struct CamIA10_Results* ia_results);
  virtual bool initISPStream(const char* ispDev);
  virtual bool threadLoop();

 private:
  struct cifisp_stat_buffer* mIspStats[CAM_ISP_NUM_OF_STAT_BUFS];
  struct CamIA10_Results mIA_results;
  V4l2Isp10Ioctl* mIspIoctl;
  struct CamIsp10Config mIspCfg;
  //have been defined in CamIsp1xCtrItf
  //CamHwItf* mCamHwItf;
  int mLaserFd;

  bool mNightMode;
  enum HAL_DAYNIGHT_MODE mDayNightSwitch;
  enum HAL_AE_STATE mAeState;
};


#endif

