
function toggleNotice() {
	var notice = document.getElementById('centralNotice');
	if (!wgNoticeToggleState) {
		notice.className = notice.className.replace('collapsed', 'expanded');
		toggleNoticeCookie('0');
	} else {
		notice.className = notice.className.replace('expanded', 'collapsed');
		toggleNoticeCookie('1');
	}
	wgNoticeToggleState = !wgNoticeToggleState;
}
function toggleNoticeStyle(elems, display) {
	if(elems)
		for(var i=0;i<elems.length;i++)
			elems[i].style.display = display;
}
function toggleNoticeCookie(state) {
	var e = new Date();
	e.setTime( e.getTime() + (7*24*60*60*1000) ); // one week
	var work='hidesnmessage='+state+'; expires=' + e.toGMTString() + '; path=/';
	document.cookie = work;
}
function pickTemplate(templates, weights) {
	var weightedTemplates = new Array();
	var currentTemplate = 0;
	var totalWeight = 0;

	if (templates.length == 0)
		return '';

	while (currentTemplate < templates.length) {
		totalWeight += weights[currentTemplate];
		for (i=0; i<weights[currentTemplate]; i++) {
			weightedTemplates[weightedTemplates.length] = templates[currentTemplate];
		}
		currentTemplate++;
	}

	if (totalWeight == 0)
		return '';

	var randomnumber=Math.floor(Math.random()*totalWeight);
	return weightedTemplates[randomnumber];
}


var wgNoticeToggleState = (document.cookie.indexOf('hidesnmessage=1')==-1);
document.writeln("\x3cstyle type=\"text/css\"\x3e\n#centralNotice .siteNoticeSmall{display:none;}\n#centralNotice .siteNoticeSmallAnon{display:none;}\n#centralNotice .siteNoticeSmallUser{display:none;}\n#centralNotice.collapsed .siteNoticeBig{display:none;}\n#centralNotice.collapsed .siteNoticeSmall{display:block;}\n#centralNotice.collapsed .siteNoticeSmallUser{display:block;}\n#centralNotice.collapsed .siteNoticeSmallAnon{display:block;}\n#centralNotice.anonnotice .siteNoticeSmallUser{display:none !important;}\n#centralNotice.usernotice .siteNoticeSmallAnon{display:none !important;}\n\x3c/style\x3e");

wgNotice=pickTemplate(["\x3cstyle type=\"text/css\"\x3e \n/* Styles for Notices */\n\n.notice-wrapper-vector_transition, .notice-collapsed-wrapper-vector_transition {\n margin: 2px auto 0;\n width: 100%;\n padding: 0;\n font-family: \'Arial\',\'Helvetica\',\'Tahoma\',sans-serif;\n color: #333;\n background-color: #ddd;\n font-size: 1em;\n font-weight: 200;\n vertical-align:middle;\n}\n\n.notice-wrapper-vector_transition {\n border: 1px solid #bbb;\n background-color: #fcfcfc;\n text-align: left;\n font-size: 1em;\n}\n\n.notice-wrapper-vector_transition a {\n color: #006699;\n}\n\n.toggle-box {\n text-align: right;\n font-size: 0.8em;\n padding: 0;\n}\n\n.trans-box {\n text-align: right;\n font-size: 0.8em;\n padding: 0;\n white-space: nowrap;\n}\n\ntd#table-cell-1 {\n margin: 0 auto;\n padding: 1px 0 3px 5px;\n width: 30px;\n}\n\ntd#table-cell-2 {\n margin: 0 auto;\n padding: 0 5px 0 0;\n font-size: 1.3em;\n}\n\n.line-ht-fix {\n line-height: 1em;\n}\n\n#centralNotice.collapsed .siteNoticeUser {\n display:none;\n}\n\ndiv.notice-links-vector_transition {\n text-align:right;\n}\n\x3c/style\x3e\n\n\x3ctable class=\"siteNoticeUser notice-wrapper-vector_transition\"\x3e\n \x3ctr\x3e\n  \x3ctd id=\"table-cell-2\" align=\"center\"\x3e\n   \x3cspan\x3e\n    Заметили что-то новое? Мы немного улучшили Википедию. \x3ca href=\"http://ru.wikipedia.org/wiki/Special:UsabilityInitiativePrefSwitch\"\x3eПодробнее.\x3c/a\x3e\n   \x3c/span\x3e\n  \x3c/td\x3e\n  \x3ctd id=\"table-cell-3\" class=\"line-ht-fix\"\x3e\n      \x3cspan class=\"toggle-box\"\x3e\n   [\x3ca href=\"#\" onclick=\"toggleNotice();return false\"\x3eСвернуть\x3c/a\x3e]\n \x3c/span\x3e\n  \x3c/td\x3e\n \x3c/tr\x3e\n\x3c/table\x3e", "\x3cstyle type=\"text/css\"\x3e\n#centralNotice.collapsed .central-notice-banner {\n display: none;\n}\n.central-notice-banner {\n position: relative;\n margin-bottom: 1em !important;\n}\n.toggle-box {\n position: absolute;\n right: 0.25em;\n top: 1.3em;\n font-size: 0.8em;\n}\n.vector-banner-base {\n height: 3em;\n background-color: white;\n background-image: url(http://upload.wikimedia.org/centralnotice/enwiki/vector/base.png);\n background-repeat: repeat-x;\n background-position: center bottom;\n}\n.vector-banner-base-left {\n height: 3em;\n background-image: url(http://upload.wikimedia.org/centralnotice/enwiki/vector/left.png);\n background-repeat: no-repeat;\n background-position: left bottom;\n}\n.vector-banner-base-right {\n height: 3em;\n background-image: url(http://upload.wikimedia.org/centralnotice/enwiki/vector/right.png);\n background-repeat: no-repeat;\n background-position: right bottom;\n}\n.vector-banner-message {\n text-align: center;\n height: 3em;\n}\n.vector-banner-message a {\n display: inline-block;\n height: 2em;\n padding: 0 1em !important;\n padding-top: 1em !important;\n cursor: pointer;\n text-decoration: none;\n}\n.vector-banner-message a.vector-banner-message-left {\n background-image: url(http://upload.wikimedia.org/centralnotice/enwiki/vector/break.png) !important;\n background-repeat: no-repeat !important;\n background-position: right bottom !important;\n color: #333333;\n}\n.vector-banner-message a.vector-banner-message-right {\n color: #0645AD;\n}\n.vector-banner-message a.vector-banner-message-right:hover {\n text-decoration: underline;\n}\n\x3c/style\x3e\n\x3cdiv class=\"central-notice-banner\"\x3e\n \x3cdiv class=\"toggle-box\"\x3e\x3ca href=\"#\" onclick=\"toggleNotice();return false\"\x3eСвернуть\x3c/a\x3e\x3c/div\x3e\n \x3cdiv class=\"vector-banner-base\"\x3e\n  \x3cdiv class=\"vector-banner-base-left\"\x3e\n   \x3cdiv class=\"vector-banner-base-right\"\x3e\n    \x3cdiv class=\"vector-banner-message\"\x3e\n     \x3ca href=\"http://ru.wikipedia.org/wiki/%D0%92%D0%B8%D0%BA%D0%B8%D0%BF%D0%B5%D0%B4%D0%B8%D1%8F:%D0%9E%D1%82%D0%B7%D1%8B%D0%B2%D1%8B_%D0%BE_%D0%BD%D0%BE%D0%B2%D0%BE%D0%BC_%D0%BE%D1%84%D0%BE%D1%80%D0%BC%D0%BB%D0%B5%D0%BD%D0%B8%D0%B8/%D0%9D%D0%BE%D0%B2%D1%8B%D0%B5_%D0%B2%D0%BE%D0%B7%D0%BC%D0%BE%D0%B6%D0%BD%D0%BE%D1%81%D1%82%D0%B8\" class=\"vector-banner-message-left\"\x3eВикипедия обретает новый вид.\x3c/a\x3e\x3ca href=\"http://ru.wikipedia.org/wiki/%D0%92%D0%B8%D0%BA%D0%B8%D0%BF%D0%B5%D0%B4%D0%B8%D1%8F:%D0%9E%D1%82%D0%B7%D1%8B%D0%B2%D1%8B_%D0%BE_%D0%BD%D0%BE%D0%B2%D0%BE%D0%BC_%D0%BE%D1%84%D0%BE%D1%80%D0%BC%D0%BB%D0%B5%D0%BD%D0%B8%D0%B8/%D0%9D%D0%BE%D0%B2%D1%8B%D0%B5_%D0%B2%D0%BE%D0%B7%D0%BC%D0%BE%D0%B6%D0%BD%D0%BE%D1%81%D1%82%D0%B8\" class=\"vector-banner-message-right\"\x3eПодробнее.\x3c/a\x3e\n    \x3c/div\x3e\n   \x3c/div\x3e\n  \x3c/div\x3e\n \x3c/div\x3e\n\x3c/div\x3e"],[100, 0]);
if (wgNotice != '')
wgNotice='<div id="centralNotice" class="' + (wgNoticeToggleState ? 'expanded' : 'collapsed') + ' ' + (wgUserName ? 'usernotice' : 'anonnotice' ) + '">' + wgNotice+'</div>';
