package cn.sharesdk;

import java.util.HashMap;
import org.cocos2dx.lib.Cocos2dxActivity;
import cn.sharesdk.framework.Platform;
import cn.sharesdk.framework.PlatformActionListener;
import cn.sharesdk.framework.ShareSDK;
import cn.sharesdk.framework.utils.UIHandler;
import android.content.Context;

public class ShareSDKUtils {
	private static Context context;
	private static PlatformActionListener paListaner 
		= new PlatformActionListener() {
		public void onError(Platform platform, int action, Throwable t) {
			ShareSDKUtils.onError(platform, action, t);
		}
		
		public void onComplete(Platform platform, int action, HashMap<String, Object> res) {
			ShareSDKUtils.onComplete(platform, action, res);
		}
		
		public void onCancel(Platform platform, int action) {
			ShareSDKUtils.onCancel(platform, action);
		}
	};
	
	public static void prepare() {
		UIHandler.prepare();
		if (context == null) {
			context = Cocos2dxActivity.getContext().getApplicationContext();
		}
	}
	
	public static void initSDK(String appKey, boolean networkDevInfoEnable) {
		if (networkDevInfoEnable) {
			ShareSDK.initSDK(context, appKey);
			ShareSDK.setNetworkDevInfoEnable(true);
		} else {
			ShareSDK.initSDK(context);
		}
	}
	
	public static void stopSDK() {
		ShareSDK.stopSDK(context);
	}
	
	private ShareSDKUtils() {
		
	}

	private static void onCancel(Platform platform, int action) {
		
	}

	private static void onComplete(Platform platform, int action, HashMap<String, Object> res) {
		
	}

	private static void onError(Platform platform, int action, Throwable t) {
		
	}
	
}
