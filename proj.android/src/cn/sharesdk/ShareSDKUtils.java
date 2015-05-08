package cn.sharesdk;

import java.util.ArrayList;
import java.util.HashMap;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.plugin.PluginWrapper;

import android.content.Context;
import android.os.Handler.Callback;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import android.widget.Toast;
import cn.sharesdk.framework.Platform;
import cn.sharesdk.framework.Platform.ShareParams;
import cn.sharesdk.framework.PlatformActionListener;
import cn.sharesdk.framework.PlatformDb;
import cn.sharesdk.framework.ShareSDK;
import cn.sharesdk.onekeyshare.OnekeyShare;

import com.mob.tools.utils.Hashon;
import com.mob.tools.utils.UIHandler;

public class ShareSDKUtils {
	private static boolean DEBUG = true;
	private static Context context;
	private static PlatformActionListener paListaner;
	private static Hashon hashon;

	private ShareSDKUtils() {

	}

	public static void prepare() {
		context = Cocos2dxActivity.getContext().getApplicationContext();
		hashon = new Hashon();
		final Callback cb = new Callback() {
			public boolean handleMessage(final Message msg) {
				PluginWrapper.runOnGLThread(new Runnable() {
					public void run() {
						onJavaCallback((String) msg.obj);
					}
				});

				return false;
			}
		};
		paListaner = new PlatformActionListener() {
			public void onComplete(Platform platform, int action,
					HashMap<String, Object> res) {
				if (DEBUG) {
					System.out.println("onComplete");
					System.out.println(res == null ? "" : res.toString());
				} 
				
				HashMap<String, Object> map = new HashMap<String, Object>();
				map.put("platform",
						ShareSDK.platformNameToId(platform.getName()));
				map.put("action", action);
				map.put("status", 1); // Success = 1, Fail = 2, Cancel = 3
				if(action == 8 || action == 2){
					//action = 2, 8, 获取用户列表与用户信息才放入   
					map.put("res", res);
				}
				map.put("platformDb", getPlatformDB(platform));
				Message msg = new Message();
				msg.obj = hashon.fromHashMap(map);
				UIHandler.sendMessage(msg, cb);
			}

			public void onError(Platform platform, int action, Throwable t) {
				if (DEBUG) {
					System.out.println("onError");
					t.printStackTrace();
				}
				HashMap<String, Object> map = new HashMap<String, Object>();
				map.put("platform",
						ShareSDK.platformNameToId(platform.getName()));
				map.put("action", action);
				map.put("status", 2); // Success = 1, Fail = 2, Cancel = 3
				map.put("res", throwableToMap(t));
				Message msg = new Message();
				msg.obj = hashon.fromHashMap(map);
				UIHandler.sendMessage(msg, cb);
			}

			public void onCancel(Platform platform, int action) {
				if (DEBUG) {
					System.out.println("onCancel");
				}
				HashMap<String, Object> map = new HashMap<String, Object>();
				map.put("platform",
						ShareSDK.platformNameToId(platform.getName()));
				map.put("action", action);
				map.put("status", 3); // Success = 1, Fail = 2, Cancel = 3
				Message msg = new Message();
				msg.obj = hashon.fromHashMap(map);
				UIHandler.sendMessage(msg, cb);
			}
		};
	}

	private static native void onJavaCallback(String resp);

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

	public static void initSDK(final String appKey,
			final boolean enableStatistics) {
		if (DEBUG) {
			System.out.println("initSDK");
		}
		UIHandler.sendEmptyMessage(1, new Callback() {
			@Override
			public boolean handleMessage(Message msg) {
				ShareSDK.initSDK(context, appKey, enableStatistics);
				return true;
			}
		});
	}

	public static void stopSDK() {
		if (DEBUG) {
			System.out.println("stopSDK");
		}
		ShareSDK.stopSDK(context);
	}

	public static void setPlatformConfig(int platformId, String configs) {
		if (DEBUG) {
			System.out.println("setPlatformConfig");
		}
		String name = ShareSDK.platformIdToName(platformId);
		HashMap<String, Object> conf = new Hashon().fromJson(configs);
		ShareSDK.setPlatformDevInfo(name, conf);
	}
	
	private static HashMap<String, Object> getPlatformDB(Platform platform){
		HashMap<String, Object> platformDbMap = new HashMap<String, Object>();
		PlatformDb db = platform.getDb();
		platformDbMap.put("token", db.getToken());
		platformDbMap.put("userGender", db.getUserGender());
		platformDbMap.put("userId", db.getUserId());
		platformDbMap.put("userName", db.getUserName());
		platformDbMap.put("userIcon", db.getUserIcon());
		platformDbMap.put("platformName", db.getPlatformNname());
		platformDbMap.put("platformVersion", db.getPlatformVersion());
		return platformDbMap;
	}
	
	public static String getAuthInfo(int platformId){
		if (DEBUG) {
			System.out.println("getAuthInfo");
		}
		HashMap<String, Object> userInfo = new HashMap<String, Object>();
		String name = ShareSDK.platformIdToName(platformId);
		Platform plat = ShareSDK.getPlatform(context, name);
		if(plat.isValid()){
			userInfo = getPlatformDB(plat);
		}
		return ((String)hashon.fromHashMap(userInfo));
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
	
	public static void listFriend(int platformId, int count, int page, String account){
		if (DEBUG) {
			System.out.println("listFriend");
			System.out.println("count:" + count + " page:" + page + " account:" + account);
		}
		String name = ShareSDK.platformIdToName(platformId);
		Platform plat = ShareSDK.getPlatform(context, name);
		plat.setPlatformActionListener(paListaner);
		plat.SSOSetting(true);
		plat.listFriend(count, page, account);
	}
	
	public static void followFriend(int platformId, String account){
		if (DEBUG) {
			System.out.println("followFriend");
		}
		String name = ShareSDK.platformIdToName(platformId);
		Platform plat = ShareSDK.getPlatform(context, name);
		plat.setPlatformActionListener(paListaner);
		plat.followFriend(account);
	}


	public static void share(int platformId, String contentJson) {
		if (DEBUG) {
			System.out.println("share");
		}
		String name = ShareSDK.platformIdToName(platformId);
		Platform plat = ShareSDK.getPlatform(context, name);
		plat.setPlatformActionListener(paListaner);
		try {
			HashMap<String, Object> content = hashon.fromJson(contentJson);
			content = nativeMapToJavaMap(content);
			ShareParams sp = new ShareParams(content);
			plat.share(sp);
		} catch (Throwable t) {
			paListaner.onError(plat, Platform.ACTION_SHARE, t);
		}
	}

	private static HashMap<String, Object> nativeMapToJavaMap(
			HashMap<String, Object> content) {
		HashMap<String, Object> map = new HashMap<String, Object>();
		if (!(content.get("content") == null)) {
			map.put("text", content.get("content"));
		}
		if (!(content.get("image") == null)) {
			String image = (String) content.get("image");
			if (image != null && image.startsWith("/")) {
				map.put("imagePath", image);
			} else if (!TextUtils.isEmpty(image)) {
				map.put("imageUrl", image);
			}
		}
		if (!(content.get("title") == null)) {
			map.put("title", content.get("title"));
		}
		if (!(content.get("description") == null)) {
			map.put("comment", content.get("description"));
		}
		if (!(content.get("url") == null)) {
			map.put("url", content.get("url"));
			map.put("titleUrl", content.get("url"));
		}
		if (!(content.get("site") == null)) {
			map.put("site", content.get("site"));
		}
		if (!(content.get("siteUrl") == null)) {
			map.put("siteUrl", content.get("siteUrl"));
		}
		if (!(content.get("musicUrl") == null)) {
			map.put("musicUrl", content.get("musicUrl"));
		}
		if (!(content.get("extInfo") == null)) {
			map.put("extInfo", content.get("extInfo"));
		}
		String type = (String) content.get("type");
		if (type != null) {
			int shareType = iosTypeToAndroidType(Integer.parseInt(type));
			map.put("shareType", shareType);
		}
		return map;
	}

	
	
	private static int iosTypeToAndroidType(int type) {
		switch (type) {
		case 1:
			return Platform.SHARE_IMAGE;
		case 2:
			return Platform.SHARE_WEBPAGE;
		case 3:
			return Platform.SHARE_MUSIC;
		case 4:
			return Platform.SHARE_VIDEO;
		case 5:
			return Platform.SHARE_APPS;
		case 6:
		case 7:
			return Platform.SHARE_EMOJI;
		case 8:
			return Platform.SHARE_FILE;
		}
		return Platform.SHARE_TEXT;
	}

	public static void onekeyShare(String contentJson) {
		onekeyShare(0, contentJson);
	}

	public static void onekeyShare(int platformId, String contentJson) {
		if (DEBUG) {
			System.out.println("OnekeyShare");
		}
		HashMap<String, Object> content = hashon.fromJson(contentJson);
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
		if (map.containsKey("titleUrl")) {
			oks.setTitleUrl(String.valueOf(map.get("titleUrl")));
		}
		if (map.containsKey("site")) {
			oks.setSite(String.valueOf(map.get("site")));
		}
		if (map.containsKey("siteUrl")) {
			oks.setSiteUrl(String.valueOf(map.get("siteUrl")));
		}
		oks.setCallback(paListaner);
		if (platformId > 0) {
			oks.setPlatform(ShareSDK.platformIdToName(platformId));
		}
		oks.show(context);
	}

	public static void toast(final String message) {
		Log.e(ShareSDKUtils.class.getName(), message);
		UIHandler.sendEmptyMessage(1, new Callback() {
			@Override
			public boolean handleMessage(Message msg) {
				Toast.makeText(context, message, Toast.LENGTH_LONG).show();
				return true;
			}
		});
	}
}
