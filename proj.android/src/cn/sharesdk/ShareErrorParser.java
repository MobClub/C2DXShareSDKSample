package cn.sharesdk;

import java.util.HashMap;

import org.json.JSONException;
import org.json.JSONObject;

public class ShareErrorParser {
	public static final int C2DXPlatTypeSinaWeibo = 1;
	/** < 新浪微博 */
	public static final int C2DXPlatTypeTencentWeibo = 2;
	/** < 腾讯微博 */
	public static final int C2DXPlatTypeSohuWeibo = 3;
	/** < 搜狐微博 */
	public static final int C2DXPlatType163Weibo = 4;
	/** < 网易微博 */
	public static final int C2DXPlatTypeDouBan = 5;
	/** < 豆瓣社区 */
	public static final int C2DXPlatTypeQZone = 6;
	/** < QQ空间 */
	public static final int C2DXPlatTypeRenren = 7;
	/** < 人人网 */
	public static final int C2DXPlatTypeKaixin = 8;
	/** < 开心网 */
	public static final int C2DXPlatTypePengyou = 9;
	/** < 朋友网 */
	public static final int C2DXPlatTypeFacebook = 10;
	/** < Facebook */
	public static final int C2DXPlatTypeTwitter = 11;
	/** < Twitter */
	public static final int C2DXPlatTypeEvernote = 12;
	/** < 印象笔记 */
	public static final int C2DXPlatTypeFoursquare = 13;
	/** < Foursquare */
	public static final int C2DXPlatTypeGooglePlus = 14;
	/** < Google＋ */
	public static final int C2DXPlatTypeInstagram = 15;
	/** < Instagram */
	public static final int C2DXPlatTypeLinkedIn = 16;
	/** < LinkedIn */
	public static final int C2DXPlatTypeTumblr = 17;
	/** < Tumbir */
	public static final int C2DXPlatTypeMail = 18;
	/** < 邮件分享 */
	public static final int C2DXPlatTypeSMS = 19;
	/** < 短信分享 */
	public static final int C2DXPlatTypeAirPrint = 20;
	/** < 打印 */
	public static final int C2DXPlatTypeCopy = 21;
	/** < 拷贝 */
	public static final int C2DXPlatTypeWeixiSession = 22;
	/** < 微信好友 */
	public static final int C2DXPlatTypeWeixiTimeline = 23;
	/** < 微信朋友圈 */
	public static final int C2DXPlatTypeQQ = 24;
	/** < QQ */
	public static final int C2DXPlatTypeInstapaper = 25;
	/** < Instapaper */
	public static final int C2DXPlatTypePocket = 26;
	/** < Pocket */
	public static final int C2DXPlatTypeYouDaoNote = 27;
	/** < 有道云笔记 */
	public static final int C2DXPlatTypeSohuKan = 28;
	/** < 搜狐随身看 */
	public static final int C2DXPlatTypePinterest = 30;
	/** < Pinterest */
	public static final int C2DXPlatTypeFlickr = 34;
	/** < Flickr */
	public static final int C2DXPlatTypeDropbox = 35;
	/** < Dropbox */
	public static final int C2DXPlatTypeVKontakte = 36;
	/** < VKontakte */
	public static final int C2DXPlatTypeWeixiFav = 37;
	/** < 微信收藏 */
	public static final int C2DXPlatTypeYiXinSession = 38;
	/** < 易信好友 */
	public static final int C2DXPlatTypeYiXinTimeline = 39;
	/** < 易信朋友圈 */
	public static final int C2DXPlatTypeYiXinFav = 40;
	/** < 易信收藏 */
	public static final int C2DXPlatTypeAny = 99;
	/** < 任意平台 */

	public static HashMap<String, String> parse(int platType, String msg) {
		HashMap<String, String> map = new HashMap<String, String>();
		JSONObject o = null;
		try {
			o = new JSONObject(msg);
		} catch (JSONException e) {
			e.printStackTrace();
			return map;
		}

		try {
			switch (platType) {
			case C2DXPlatTypeSinaWeibo:
				parseSinaWeibo(map, o);
				break;
			}
		} catch (JSONException e) {
			e.printStackTrace();
		}
		return map;
	}

	private static void parseSinaWeibo(HashMap<String, String> map, JSONObject o)
			throws JSONException {
		map.put("error_code", o.getString("error_code"));
		map.put("error_msg", o.getString("error"));
	}

}
