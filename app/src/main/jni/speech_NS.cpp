# include <speech_NS.h>
//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <android/log.h>
#include "./webrtc_ns/signal_processing_library.h"
#include "./webrtc_ns/noise_suppression_x.h"
#include "./webrtc_ns/noise_suppression.h"

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "WebRTC", __VA_ARGS__)

JNIEXPORT void JNICALL Java_speech_NS_noiseSuppression(JNIEnv *env,
		jclass jcls, jstring jfile_in, jstring jfile_out, jint jsample,jint jmode) {

	const char *szFileIn = env->GetStringUTFChars(jfile_in, NULL);
	const char *szFileOut = env->GetStringUTFChars(jfile_out, NULL);

	int nRet = 0;
	NsHandle *pNS_inst = NULL;

	FILE *fpIn = NULL;
	FILE *fpOut = NULL;

	char *pInBuffer = NULL;
	char *pOutBuffer = NULL;
	int i = 0;
	int nFileSize = 0;
	WebRtcNs_Create(&pNS_inst);
	WebRtcNs_Init(pNS_inst, jsample);
	WebRtcNs_set_policy(pNS_inst, jmode);

	fpIn = fopen(szFileIn, "rb");
	if (NULL == fpIn) {
		LOGE("open src file err \n");
		WebRtcNs_Free(pNS_inst);
		fclose(fpIn);
		return;
	}
	fseek(fpIn, 0, SEEK_END);
	nFileSize = ftell(fpIn);
	fseek(fpIn, 0, SEEK_SET);

	pInBuffer = (char*) malloc(nFileSize);
	memset(pInBuffer, 0, nFileSize);
	fread(pInBuffer, sizeof(char), nFileSize, fpIn);

	pOutBuffer = (char*) malloc(nFileSize);
	memset(pOutBuffer, 0, nFileSize);

	int filter_state1[6], filter_state12[6];
	int Synthesis_state1[6], Synthesis_state12[6];

	memset(filter_state1, 0, sizeof(filter_state1));
	memset(filter_state12, 0, sizeof(filter_state12));
	memset(Synthesis_state1, 0, sizeof(Synthesis_state1));
	memset(Synthesis_state12, 0, sizeof(Synthesis_state12));

	for (i = 0; i < nFileSize; i += 640) {
		if (nFileSize - i >= 640) {
			short shBufferIn[320] = { 0 };

			short shInL[160], shInH[160];
			short shOutL[160] = { 0 }, shOutH[160] = { 0 };

			memcpy(shBufferIn, (char*) (pInBuffer + i), 320 * sizeof(short));
			//首先需要使用滤波函数将音频数据分高低频，以高频和低频的方式传入降噪函数内部
			WebRtcSpl_AnalysisQMF(shBufferIn, 320, shInL, shInH, filter_state1,
					filter_state12);

			//将需要降噪的数据以高频和低频传入对应接口，同时需要注意返回数据也是分高频和低频
			if (0 == WebRtcNs_Process(pNS_inst, shInL, shInH, shOutL, shOutH)) {
				short shBufferOut[320];
				//如果降噪成功，则根据降噪后高频和低频数据传入滤波接口，然后用将返回的数据写入文件
				WebRtcSpl_SynthesisQMF(shOutL, shOutH, 160, shBufferOut,
						Synthesis_state1, Synthesis_state12);
				memcpy(pOutBuffer + i, shBufferOut, 320 * sizeof(short));
			}
		}
	}

	fpOut = fopen(szFileOut, "wb");
	if (NULL == fpOut) {
		LOGE("open out file err! \n");
	} else {
		fwrite(pOutBuffer, sizeof(char), nFileSize, fpOut);
	}
	WebRtcNs_Free(pNS_inst);
	fclose(fpIn);
	fclose(fpOut);
	free(pInBuffer);
	free(pOutBuffer);
}
JNIEXPORT void JNICALL Java_speech_NS_noiseSuppressionByBytes(JNIEnv *env,
		jclass jcls, jbyteArray jbuffer,jint jsample,jint jmode) {

	jbyte* buffer = env->GetByteArrayElements(jbuffer,NULL);

	int size = env->GetArrayLength(jbuffer);

	int nRet = 0;
	NsHandle *pNS_inst = NULL;

	FILE *fpIn = NULL;
	FILE *fpOut = NULL;

	WebRtcNs_Create(&pNS_inst);
	WebRtcNs_Init(pNS_inst, jsample);
	//WebRtcNs_set_policy中的第二个参数mode 抑制噪声级别，2的时候最高
	WebRtcNs_set_policy(pNS_inst, jmode);


	int filter_state1[6], filter_state12[6];
	int Synthesis_state1[6], Synthesis_state12[6];

	memset(filter_state1, 0, sizeof(filter_state1));
	memset(filter_state12, 0, sizeof(filter_state12));
	memset(Synthesis_state1, 0, sizeof(Synthesis_state1));
	memset(Synthesis_state12, 0, sizeof(Synthesis_state12));

	int i = 0;
	for (i = 0; i < size; i += 640) {
		if (size - i >= 640) {
			short shBufferIn[320] = { 0 };

			short shInL[160], shInH[160];
			short shOutL[160] = { 0 }, shOutH[160] = { 0 };

			memcpy(shBufferIn, (char*) (buffer + i), 320 * sizeof(short));
			//首先需要使用滤波函数将音频数据分高低频，以高频和低频的方式传入降噪函数内部
			WebRtcSpl_AnalysisQMF(shBufferIn, 320, shInL, shInH, filter_state1,
					filter_state12);

			//将需要降噪的数据以高频和低频传入对应接口，同时需要注意返回数据也是分高频和低频
			if (0 == WebRtcNs_Process(pNS_inst, shInL, shInH, shOutL, shOutH)) {
				short shBufferOut[320];
				//如果降噪成功，则根据降噪后高频和低频数据传入滤波接口，然后用将返回的数据写入文件
				WebRtcSpl_SynthesisQMF(shOutL, shOutH, 160, shBufferOut,
						Synthesis_state1, Synthesis_state12);
				memcpy(buffer + i, shBufferOut, 320 * sizeof(short));
			}
		}
	}

	WebRtcNs_Free(pNS_inst);
	env->ReleaseByteArrayElements(jbuffer,buffer,JNI_COMMIT);
}
