/** global */
var rtn4p_item          = 3490;
var rtn4p_show_src      = 0;
var rtn4p_show_desc     = 0;
var rtn4p_show_photo    = 1;
var rtn4p_show_align    = 'left';
/** styles */
var rtn4p_class_title   = 'rttitle';
var rtn4p_class_src     = '';
var rtn4p_class_desc    = '';
var rtn4p_class_photo   = '';
var rtn4p_style_photo   = 'margin:0px 5px 10px 0px; width:60px; height:60px; border:1px solid #cccccc; float:left;';
/** header/footer */
var rtn4p_header        = '<table cellspacing="0" cellpadding="0" border="0" width="100%" style="">';
var rtn4p_footer        = '<'+'/table><img src="http://nc.redtram.com/px/588.gif" alt="" width="1" height="1" border="0" />';
/** news line */
var rtn4p_line_before   = '<tr><td align="left">';
var rtn4p_line_after    = '<'+'/td></tr>';

var rtn4p_title_before  = '<div style="">';
var rtn4p_title_after   = '<'+'/div>';

var rtn4p_src_before    = '';
var rtn4p_src_after     = '';

var rtn4p_desc_before   = '';
var rtn4p_desc_after    = '';

var rtn4p_domain        = 'http://ru.redtram.com/';
var rtn4p_host          = 'http://n4p.ru.redtram.com/';
var rtn4p_photo         = 'http://img22.ru.redtram.com/news/';
var rtn4p_page          = 1;

var rtn4p_data          = '';
var rtn4p_css_styles    = 'a.rttitle:link,a.rttitle:active,a.rttitle:hover,a.rttitle:visited{FONT-SIZE: 11px;}';

var rtn4p_initid        = 'rtn4pogr';

var ak = Array();	
var count_ak = 0;	
	
/** do not edit */
function getCookie(name){var dc=document.cookie;var prefix=name+"=";var begin=dc.indexOf("; "+prefix);if(begin==-1){begin=dc.indexOf(prefix);if(begin!=0){return null;}}else{begin += 2;}var end=dc.indexOf(";", begin);if (end==-1){end=dc.length;}return unescape(dc.substring(begin+prefix.length,end));}
function setCookie(name,value,expires,path,domain,secure){document.cookie=name+"="+escape(value)+((expires)?"; expires="+expires.toGMTString():"")+((path)?"; path="+path:"")+((domain)?";domain="+domain:"")+((secure)?"; secure":"");}
function RedTramCookies(value){var cn="rtn4p";if(value){rtn4p_page=value;}else{var c=getCookie(cn);if(c!=null&&parseInt(c)<=10&&(parseInt(c)+1)<=10){rtn4p_page=parseInt(c)+1;}setCookie(cn,rtn4p_page,"","/");}}RedTramCookies();

var rtn4p_init          = document.getElementById(rtn4p_initid);

function RedTramI(){if(rtn4p_init){rtn4p_init.innerHTML = rtn4p_data;}}
function RedTramH(){rtn4p_data+=rtn4p_header;}
function RedTramF(){rtn4p_data+=rtn4p_footer;RedTramI();}
function RedTramAdd(title,url,src,desc,photo,special){
    str=rtn4p_line_before;
    if(rtn4p_show_src){str+=rtn4p_src_before+'<a target="_blank" href="'+rtn4p_domain+'sources/'+src+'/"'+(rtn4p_class_src?' class="'+rtn4p_class_src+'"':'')+'>'+src+'<'+'/a>'+rtn4p_src_after;}
    if(rtn4p_show_photo&&photo!=''){str+='<a target="_blank" href="'+rtn4p_domain+'go/'+url+'/n4p/'+rtn4p_item+'?q='+ak[count_ak]+'"><img'+(rtn4p_class_photo?' class="'+rtn4p_class_photo+'"':'')+' src="'+rtn4p_photo+''+photo+'" border=0 align="'+rtn4p_show_align+'"'+(rtn4p_style_photo?' style="'+rtn4p_style_photo+'"':'')+' /><'+'/a>';}
    str+=rtn4p_title_before+'<a target="_blank" href="'+rtn4p_domain+'go/'+url+'/n4p/'+rtn4p_item+'?q='+ak[count_ak]+'"'+(rtn4p_class_title?' class="'+rtn4p_class_title+'"':'')+'>'+title+'<'+'/a>'+rtn4p_title_after;
	if(rtn4p_show_desc&&desc!=''){str+=rtn4p_desc_before+'<a target="_blank" href="'+rtn4p_domain+'go/'+url+'/n4p/'+rtn4p_item+'?q='+ak[count_ak]+'"'+(rtn4p_class_desc?' class="'+rtn4p_class_desc+'"':'')+'>'+desc+'<'+'/a>'+rtn4p_desc_after;}
    rtn4p_data+=str+rtn4p_line_after;

    count_ak = count_ak + 1;
}

if (rtn4p_init) {
    document.write('<style type="text/css">'+rtn4p_css_styles+'<'+'/style>');
	document.write('<scr'+'ipt language="javascript" type="text/javascript" src="'+rtn4p_host+'?i='+rtn4p_item+'&p='+rtn4p_page+'"><'+'/scr'+'ipt>');
} 