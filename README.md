# [WebRTC 音频降噪](https://github.com/adzcsx2/Android-NoiseSupression-) #

利用 WebRTC 音频处理模块对 **PCM语音数据**（AudioRecord 录制出的裸数据） 进行降噪处理，然后播放测试降噪效果（AudioTrack 播放PCM语音数据）

## 编译说明 ##

如果需要自己编译jni代码打包so库，需要配置NDK环境。

## 降噪方法 ##

	/**
	 * 音频降噪
	 * @param file_in 输入文件
	 * @param file_out 输出文件
	 * @param sample 采样率
	 * @param mode 降噪强度0-2
	 */
	public static native void noiseSuppression(String file_in,String file_out,int sample,int mode);

	/**
	 * 音频降噪
	 * @param buffer 音频文件流
	 * @param sample 采样率
	 * @param mode   降噪强度0-2
	 */
	public static native void noiseSuppressionByBytes(byte[] buffer,int sample,int mode);



