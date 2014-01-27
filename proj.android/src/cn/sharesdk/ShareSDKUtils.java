package cn.sharesdk;

import java.lang.reflect.Field;
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
import cn.sharesdk.onekeyshare.OnekeyShare;
import android.content.Context;
import android.os.Message;
import android.os.Handler.Callback;

public class ShareSDKUtils {
	private static final String TAG = ShareSDK.class.getSimpleName();
	private static boolean DEBUG = true; 
	private static Context context;
	private static PlatformActionListener paListaner 
		= new PlatformActionListener() {
		public void onError(Platform platform, int action, Throwable t) {
			if (DEBUG) {
				System.out.println("onError");
			}
			Message msg = new Message();
			msg.what = 3;
			msg.arg1 = action;
			msg.obj = new Object[] {platform, t};
			UIHandler.sendMessage(msg, cb);
		}
		
		public void onComplete(Platform platform, int action, HashMap<String, Object> res) {
			if (DEBUG) {
				System.out.println("onComplete");
			}
			Message msg = new Message();
			msg.what = 1;
			msg.arg1 = action;
			msg.obj = new Object[] {platform, res};
			UIHandler.sendMessage(msg, cb);
		}
		
		public void onCancel(Platform platform, int action) {
			if (DEBUG) {
				System.out.println("onCancel");
			}
			Message msg = new Message();
			msg.what = 2;
			msg.arg1 = action;
			msg.obj = new Object[] {platform};
			UIHandler.sendMessage(msg, cb);
		}
	};
	private static Callback cb = new Callback() {
		@SuppressWarnings("unchecked")
		public boolean handleMessage(final Message msg) {
			switch(msg.what) {
			case 1: {
				runOnGLThread(new Runnable() {
					@Override
					public void run() {
						Object[] objs = (Object[]) msg.obj;
						ShareSDKUtils.onComplete((Platform) objs[0], msg.arg1, (HashMap<String, Object>) objs[1]);
					}
				});
			}
			break;
			case 2: {
				runOnGLThread(new Runnable() {
					@Override
					public void run() {
						Object[] objs = (Object[]) msg.obj;
						ShareSDKUtils.onCancel((Platform) objs[0], msg.arg1);
					}
				});
			}
			break;
			case 3: {
				runOnGLThread(new Runnable() {
					@Override
					public void run() {
						Object[] objs = (Object[]) msg.obj;
						ShareSDKUtils.onError((Platform) objs[0], msg.arg1, (Throwable) objs[1]);
					}
				});
			}
			break;
			}
			return false;
		}
	};
	
	private static void runOnGLThread(Runnable r) {
		Cocos2dxActivity activity =  (Cocos2dxActivity) Cocos2dxActivity.getContext();
		activity.runOnGLThread(r);
	}
	
	public static void prepare() {
		UIHandler.prepare();
		if (context == null) {
			context = Cocos2dxActivity.getContext().getApplicationContext();
		}
	}
	
	private ShareSDKUtils() {
		
	}
	
	public static void initSDK(String appKey, boolean networkDevInfoEnable) {
		if (DEBUG) {
			System.out.println("initSDK");
		}
		if (networkDevInfoEnable) {
			ShareSDK.initSDK(context, appKey);
		} else {
			ShareSDK.initSDK(context);
		}
	}
	
	public static void stopSDK() {
		if (DEBUG) {
			System.out.println("stopSDK");
		}
		ShareSDK.stopSDK(context);
	}
	
	public static void setPlatformConfig(int platformId, HashMap<String, Object> configs) {
		if (DEBUG) {
			System.out.println("setPlatformConfig");
		}
		
		if(configs.containsKey("app_key")) {
			configs.put("AppKey", configs.get("app_key"));
			configs.remove("app_key");
		}
		if(configs.containsKey("app_secret")) {
			configs.put("AppSecret", configs.get("app_secret"));
			configs.remove("app_secret");
		}
		if(configs.containsKey("redirect_uri")) {
			configs.put("RedirectUrl", configs.get("redirect_uri"));
			configs.remove("redirect_uri");
		}
    	
		String name = ShareSDK.platformIdToName(platformId);
		ShareSDK.setPlatformDevInfo(name, configs);
	}
	
	public static void authorize(int platformId) {
		if (DEBUG) {
			System.out.println("authorize");
		}
		String name = ShareSDK.platformIdToName(platformId);
		Platform plat = ShareSDK.getPlatform(context, name);
		plat.setPlatformActionListener(paListaner);
		plat.authorize();
	}
	
	public static void removeAccount(int platformId) {
		if (DEBUG) {
			System.out.println("removeAccount");
		}
		String name = ShareSDK.platformIdToName(platformId);
		Platform plat = ShareSDK.getPlatform(context, name);
		plat.removeAccount();
	}
	
	public static boolean isValid(int platformId) {
		if (DEBUG) {
			System.out.println("isValid");
		}
		String name = ShareSDK.platformIdToName(platformId);
		Platform plat = ShareSDK.getPlatform(context, name);
		return plat.isValid();
	}
	
	public static void showUser(int platformId) {
		if (DEBUG) {
			System.out.println("showUser");
		}
		String name = ShareSDK.platformIdToName(platformId);
		Platform plat = ShareSDK.getPlatform(context, name);
		plat.setPlatformActionListener(paListaner);
		plat.showUser(null);
	}
	
	public static void share(int platformId, HashMap<String, String> content) {
		if (DEBUG) {
			System.out.println("share");
		}
		String name = ShareSDK.platformIdToName(platformId);
		Platform plat = ShareSDK.getPlatform(context, name);
		plat.setPlatformActionListener(paListaner);
		try {
			ShareParams sp = hashmapToShareParams(plat, content);
			plat.share(sp);
		} catch (Throwable t) {
			paListaner.onError(plat, Platform.ACTION_SHARE, t);
		}
	}
	
	private static ShareParams hashmapToShareParams(Platform plat, 
			HashMap<String, String> content) throws Throwable {
		String className = plat.getClass().getName() + "$ShareParams";
		Class<?> cls = Class.forName(className);
		if (cls == null) {
			return null;
		}
		
		Object sp = cls.newInstance();
		if (sp == null) {
			return null;
		}
		
		HashMap<String, Object> data = nativeMapToJavaMap(content);
		for (Entry<String, Object> ent : data.entrySet()) {
			try {
				Field fld = cls.getField(ent.getKey());
				if (fld != null) {
					fld.setAccessible(true);
					fld.set(sp, ent.getValue());
				}
			} catch(Throwable t) {}
		}
		
		return (Platform.ShareParams) sp;
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
			case 1: return Platform.SHARE_IMAGE;
			case 2: return Platform.SHARE_WEBPAGE;
			case 3: return Platform.SHARE_MUSIC;
			case 4: return Platform.SHARE_VIDEO;
			case 5: return Platform.SHARE_APPS;
			case 6: 
			case 7: return Platform.SHARE_EMOJI;
			case 8: return Platform.SHARE_FILE;
		}
        return Platform.SHARE_TEXT;
	}

	public static void onekeyShare(ArrayList<Integer> platformIds, HashMap<String, String> content) {
//		Platform[] plats = ShareSDK.getPlatformList(context);
//		for (Platform p : plats) {
//			p.setDevInfoParam("Enable", "false");
//		}
//		for (int pid : platformIds) {
//			String name = ShareSDK.platformIdToName(context, pid);
//			Platform plat = ShareSDK.getPlatform(context, name);
//			plat.setDevInfoParam("Enable", "true");
//		}
		onekeyShare(content);
	}
	
	public static void onekeyShare(HashMap<String, String> content) {
		if (DEBUG) {
			System.out.println("OnekeyShare");
		}
		HashMap<String, Object> map = nativeMapToJavaMap(content);
		OnekeyShare oks = new OnekeyShare();
		if (map.containsKey("text")) {
			oks.setText(String.valueOf(map.get("text")));
		}
		if (map.containsKey("imagePath")) {
			oks.setImagePath(String.valueOf(map.get("imagePath")));
		}
		if (map.containsKey("imageUrl")) {
			oks.setImageUrl(String.valueOf(map.get("imageUrl")));
		}
		if (map.containsKey("title")) {
			oks.setTitle(String.valueOf(map.get("title")));
		}
		if (map.containsKey("comment")) {
			oks.setComment(String.valueOf(map.get("comment")));
		}
		if (map.containsKey("url")) {
			oks.setUrl(String.valueOf(map.get("url")));
		}
		oks.setCallback(paListaner);
		oks.show(context);
	}
	
	private native static void onCancel(Platform platform, int action);

	private native static void onComplete(Platform platform, int action, HashMap<String, Object> res);

	private native static void onError(Platform platform, int action, Throwable t);
	
	// =================== jni tools ================
	
	public static HashMap<String, String> newHashMap() {
		if (DEBUG) {
			System.out.println("newHashMap");
		}
		return new HashMap<String, String>();
	}
	
	public static void putData(HashMap<String, String> map, String key, String value) {
		if (DEBUG) {
			System.out.println("putData -- " + key + "=" + value);
		}
		map.put(key, value);
	}
	
	public static ArrayList<String> getMapKeys(HashMap<String, Object> map) {
		if (DEBUG) {
			System.out.println("getMapKeys");
		}
		ArrayList<String> keys = new ArrayList<String>();
		for (Entry<String, Object> ent : map.entrySet()) {
			keys.add(ent.getKey());
		}
		return keys;
	}
	
	public static Object getData(HashMap<String, Object> map, String key) {
		Object value = map.get(key);
		if (DEBUG) {
			System.out.println("getData -- " + key + "=" + value);
		}
		return value;
	}
	
	public static int platformToId(Platform platform) {
		if (DEBUG) {
			System.out.println("platformToId");
		}
		String name = platform.getName();
		return ShareSDK.platformNameToId(name);
	}
	
	public static HashMap<String, String> shareErrorToHashmap(int platformId, Throwable t) {
		return ShareErrorParser.parse(platformId, t.getMessage());
	}
	
	public static String throwableToJson(Throwable t) {
		if (DEBUG) {
			System.out.println("throwableToJson");
			t.printStackTrace();
		}
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
	
	public static ArrayList<Integer> newArrayList() {
		if (DEBUG) {
			System.out.println("newArrayList");
		}
		return new ArrayList<Integer>();
	}
	
	public static void putData(ArrayList<Integer> list, int item) {
		if (DEBUG) {
			System.out.println("putData -- " + list.size() + "=" + item);
		}
		list.add(item);
	}
	
	public static int getListSize(ArrayList<Integer> list) {
		if (DEBUG) {
			System.out.println("getListSize");
		}
		return list == null ? 0 : list.size();
	}
	
	public static String getData(ArrayList<String> list, int index) {
		String value = list.get(index);
		if (DEBUG) {
			System.out.println("getData -- " + index + "=" + value);
		}
		return value;
	}
	
	public static int getObjectType(Object data) {
		if (DEBUG) {
			System.out.println("getObjectType");
		}
		if (data instanceof Integer) {
			return 1;
		} else if(data instanceof Long) {
			return 2;
		} else if (data instanceof Double) {
			return 3;
		} else if (data instanceof Boolean) {
			return 4;
		} else if (data instanceof String) {
			return 5;
		} else if (data instanceof ArrayList) {
			return 6;
		} else if (data instanceof HashMap) {
			return 7;
		}
		return 0;
	}
	
	public static double jObjectToJDouble(Object data) {
		if (DEBUG) {
			System.out.println("jObjectToJDouble");
		}
		if (data instanceof Integer) {
			return ((Integer) data).intValue();
		} else if(data instanceof Long) {
			return ((Long) data).longValue();
		} else if (data instanceof Double) {
			return ((Double) data).doubleValue();
		} else if (data instanceof Boolean) {
			return Boolean.TRUE.equals(data) ? 1 : 0;
		}
		return 0;
	}
	
	public static int getListType(ArrayList<Object> list) {
		if (DEBUG) {
			System.out.println("getListType");
		}
		if (list.size() <= 0) {
			return 0;
		}
		
		Object item = list.get(0);
		return getObjectType(item);
	}
	
	public static Object getObjectData(ArrayList<Object> list, int index) {
		Object value = list.get(index);;
		if (DEBUG) {
			System.out.println("getObjectData -- " + index + "=" + value);
		}
		return value;
	}
	
}
