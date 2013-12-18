package cn.sharesdk;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map.Entry;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONObject;
import cn.sharesdk.framework.Platform;
import cn.sharesdk.framework.Platform.ShareParams;
import cn.sharesdk.framework.PlatformActionListener;
import cn.sharesdk.framework.ShareSDK;
import cn.sharesdk.framework.utils.UIHandler;
import android.content.Context;

public class ShareSDKUtils {
	private static Context context;
	private static PlatformActionListener paListaner 
		= new PlatformActionListener() {
		public void onError(Platform platform, int action, Throwable t) {
			System.out.println("onError");
			ShareSDKUtils.onError(platform, action, t);
		}
		
		public void onComplete(Platform platform, int action, HashMap<String, Object> res) {
			System.out.println("onComplete");
			ShareSDKUtils.onComplete(platform, action, res);
		}
		
		public void onCancel(Platform platform, int action) {
			System.out.println("onCancel");
			ShareSDKUtils.onCancel(platform, action);
		}
	};
	
	public static void prepare() {
		UIHandler.prepare();
		if (context == null) {
			context = Cocos2dxActivity.getContext().getApplicationContext();
		}
	}
	
	private ShareSDKUtils() {
		
	}
	
	public static void initSDK(String appKey, boolean networkDevInfoEnable) {
		System.out.println("initSDK");
		if (networkDevInfoEnable) {
			ShareSDK.initSDK(context, appKey);
			ShareSDK.setNetworkDevInfoEnable(true);
		} else {
			ShareSDK.initSDK(context);
		}
	}
	
	public static void stopSDK() {
		System.out.println("stopSDK");
		ShareSDK.stopSDK(context);
	}
	
	public static void setPlatformConfig(int platformId, HashMap<String, Object> configs) {
		System.out.println("setPlatformConfig");
		String name = ShareSDK.platformIdToName(context, platformId);
		Platform plat = ShareSDK.getPlatform(context, name);
		for (Entry<String, Object> ent : configs.entrySet()) {
			plat.setDevInfoParam(ent.getKey(), String.valueOf(ent.getValue()));
		}
	}
	
	public static void authorize(int platformId) {
		System.out.println("authorize");
		String name = ShareSDK.platformIdToName(context, platformId);
		Platform plat = ShareSDK.getPlatform(context, name);
		plat.setPlatformActionListener(paListaner);
		plat.authorize();
	}
	
	public static void removeAccount(int platformId) {
		System.out.println("removeAccount");
		String name = ShareSDK.platformIdToName(context, platformId);
		Platform plat = ShareSDK.getPlatform(context, name);
		plat.removeAccount();
	}
	
	public static boolean isValid(int platformId) {
		System.out.println("isValid");
		String name = ShareSDK.platformIdToName(context, platformId);
		Platform plat = ShareSDK.getPlatform(context, name);
		return plat.isValid();
	}
	
	public static void showUser(int platformId) {
		System.out.println("showUser");
		String name = ShareSDK.platformIdToName(context, platformId);
		Platform plat = ShareSDK.getPlatform(context, name);
		plat.setPlatformActionListener(paListaner);
		plat.showUser(null);
	}
	
	public static void share(int platformId, HashMap<String, String> content) {
		String name = ShareSDK.platformIdToName(context, platformId);
		Platform plat = ShareSDK.getPlatform(context, name);
		plat.setPlatformActionListener(paListaner);
		ShareParams sp = hashmapToShareParams(plat, content);
		plat.share(sp);
	}
	
	private static ShareParams hashmapToShareParams(Platform plat, HashMap<String, String> content) {
		
		return null;
	}
	
	private static HashMap<String, Object> nativeMapToJavaMap(HashMap<String, String> content) {
		HashMap<String, Object> map = new HashMap<String, Object>();
		map.put("text", content.get("content"));
		String image = content.get("image");
		if (image != null && image.startsWith("/")) {
			map.put("imagePath", image);
		} else {
			map.put("imageUrl", image);
		}
		map.put("title", content.get("title"));
		map.put("comment", content.get("description"));
		map.put("url", content.get("url"));
		String type = content.get("type");
		if (type != null) {
			int shareType = iosTypeToAndroidType(Integer.parseInt(type));
			map.put("shareType", shareType);
		}
		return map;
	}
	
	private static int iosTypeToAndroidType(int type) {
		switch (type) {
			// TODO 要IOS的类型
		}
		return 0;
	}

	public static void OnekeyShare(ArrayList<Integer> platformIds, HashMap<String, String> content) {
		Platform[] plats = ShareSDK.getPlatformList(context);
		for (Platform p : plats) {
			p.setDevInfoParam("Enable", "false");
		}
		for (int pid : platformIds) {
			String name = ShareSDK.platformIdToName(context, pid);
			Platform plat = ShareSDK.getPlatform(context, name);
			plat.setDevInfoParam("Enable", "true");
		}
		OnekeyShare(content);
	}
	
	public static void OnekeyShare(HashMap<String, String> content) {
		
	}
	
	private native static void onCancel(Platform platform, int action);

	private native static void onComplete(Platform platform, int action, HashMap<String, Object> res);

	private native static void onError(Platform platform, int action, Throwable t);
	
	// =================== jni tools ================
	
	public static HashMap<String, String> newHashMap() {
		System.out.println("newHashMap");
		return new HashMap<String, String>();
	}
	
	public static void putData(HashMap<String, String> map, String key, String value) {
		System.out.println("putData");
		map.put(key, value);
	}
	
	public static int platformToId(Platform platform) {
		System.out.println("platformToId");
		String name = platform.getName();
		return ShareSDK.platformNameToId(context, name);
	}
	
	public static String throwableToJson(Throwable t) {
		HashMap<String, Object> map = throwableToMap(t);
		return new JSONObject(map).toString();
	}
	
	private static HashMap<String, Object> throwableToMap(Throwable t) {
		HashMap<String, Object> map = new HashMap<String, Object>();
		map.put("msg", t.getMessage());
		ArrayList<HashMap<String, Object>> traces = new ArrayList<HashMap<String, Object>>();
		for (StackTraceElement trace : t.getStackTrace()) {
			HashMap<String, Object> element = new HashMap<String, Object>();
			element.put("cls", trace.getClassName());
			element.put("method", trace.getMethodName());
			element.put("file", trace.getFileName());
			element.put("line", trace.getLineNumber());
			traces.add(element);
		}
		map.put("stack", traces);
		Throwable cause = t.getCause();
		if (cause != null) {
			map.put("cause", throwableToMap(cause));
		}
		return map;
	}
	
}
