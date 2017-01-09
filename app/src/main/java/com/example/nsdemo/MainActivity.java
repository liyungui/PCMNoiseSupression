package com.example.nsdemo;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;

import com.example.test2.R;

import speech.NS;

import android.app.Activity;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.view.View.OnClickListener;

public class MainActivity extends Activity {

//	String file_in = Environment.getExternalStorageDirectory()
//			+ "/FinalAudio.wav";
//	String file_out = Environment.getExternalStorageDirectory()
//			+ "/FinalAudio23.wav";
	String file_in = AudioFileFunc.getRawFilePath();
	AudioTrack audioTrack;
	static final int frequency = 16000;
	static final int channelConfiguration = AudioFormat.CHANNEL_CONFIGURATION_MONO;
	static final int audioEncoding = AudioFormat.ENCODING_PCM_16BIT;
	int recBufSize, playBufSize;

	// 降噪
	ByteArrayOutputStream baos;
	// 未降噪
	ByteArrayOutputStream baos2;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		playBufSize = AudioTrack.getMinBufferSize(frequency,
				channelConfiguration, audioEncoding);
		audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, frequency,
				channelConfiguration, audioEncoding, playBufSize,
				AudioTrack.MODE_STREAM);
		findViewById(R.id.btn_record).setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				//录音3秒钟
				Recorder.getInstance().startRecord(3000);
			}
		});
		findViewById(R.id.btn_play1).setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				byte[] buffer;
				try {
					String file_out = Environment.getExternalStorageDirectory()+"/123.pcm";
					NS.noiseSuppression(file_in,file_out, 16000, 1);
					byte[] fileBuffer = readFile(file_out);
					audioTrack.play();
					audioTrack.write(fileBuffer, 0, fileBuffer.length - 1);
					audioTrack.stop();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		});
		findViewById(R.id.btn_play2).setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				byte[] buffer;
				try {
//					String file2 = Environment.getExternalStorageDirectory()+"/123.pcm";
//					MyRTC.noiseReduction(AudioFileFunc.getRawFilePath(), file2, 16000, 1);
					buffer = readFile(file_in);
					NS.noiseSuppressionByBytes(buffer, 16000, 2);
					audioTrack.play();
					audioTrack.write(buffer, 0, buffer.length - 1);
					audioTrack.stop();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		});
		findViewById(R.id.btn_play3).setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				byte[] buffer;
				try {
					buffer = readFile(AudioFileFunc.getRawFilePath());
					audioTrack.play();
					audioTrack.write(buffer, 0, buffer.length - 1);
					audioTrack.stop();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		});
	}

	public static byte[] readFile(String filename) throws IOException {

		File file = new File(filename);
		if (filename == null || filename.equals("")) {
			throw new NullPointerException("无效的文件路径");
		}
		long len = file.length();
		byte[] bytes = new byte[(int) len];

		BufferedInputStream bufferedInputStream = new BufferedInputStream(
				new FileInputStream(file));
		int r = bufferedInputStream.read(bytes);
		if (r != len)
			throw new IOException("读取文件不正确");
		bufferedInputStream.close();

		return bytes;

	}


}
