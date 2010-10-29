
var addthis_config = {
    data_track_clickback: true
}

var RATING_CELLS = 10;

var max_screen_games = 1;
var min_screen_games = 0;
var max_new_games = 15;
var min_new_games = 5;
var max_newfiles_games = 10;
var min_newfiles_games = 5;
var min_pop_games = 5;
var max_pop_games = 10;
var min_comments_games = 10;
var max_comments_games = 100;


function set_rating(rating)
{
    for (i = 1; i <= RATING_CELLS; i++)
    {
        if (i <= rating)
        {
            eval("rating_"+i+".className='rating_on';");
        }
        else
        {
            eval("rating_"+i+".className='rating_off';");
        }
    }
}

function rating_over(e)
{
    e = e.target || window.event.srcElement;
    rating = e.id.substring(e.id.indexOf("_") + 1);
    set_rating(rating);
}

function rating_leave(e)
{
    set_rating(rating_form.user_rating.value);
}

function rating_click(e)
{
    e = e.target || window.event.srcElement;
    rating = e.id.substring(e.id.indexOf("_") + 1);
    rating_form.user_rating.value = rating;
}

function rating_vote()
{
    if(rating_form.user_rating.value > 0)
    {
        rating_form.submit();
    }
    else
    {
        alert('Для начала выставите игре свой рейтинг');
    }
}

function leave_button(e)
{
    e = e.target || window.event.srcElement;
    if (e.id.indexOf("_table") < 0)
    {
        if (e.className.indexOf("button") >= 0)
        {
            if (e.className != "button_clicked")
            {
                e.className = "button";
            }
        }
    }
}

function over_button(e)
{
 e = e.target || window.event.srcElement;

 if (e.id.indexOf("_table") < 0)
 {
    if (e.className.indexOf("button") >= 0)
    {
        if (e.className != "button_clicked")
        {
            e.className = "button_over";
        }
    }
 }
}

function show_more(e, what)
{
    eval("MAX = max_" + what + "_games;");
    eval("MIN = min_" + what + "_games;");
    eval("checked = is_show_" + what + "_games.checked;");

    if (checked)
    {
        for(i = 1; i <= MAX; i++)
        {
                eval(what + "_game_"+i+".style.display='block';");
        }
    }
    else
    {
        for(i = MIN + 1; i <= MAX; i++)
        {
           eval(what + "_game_"+i+".style.display='none';");
        }
    }
}

function do_uploadscreens(fname,num)
{
window.open("upload.php?id="+fname,"","toolbar=0,location=0,directories=0,status=0,menubar=0,scrollbars=1,resizable=1,width=490,height=550")
}

function do_uploadvideos(fname,num)
{
window.open("upload_video.php?id="+fname,"","toolbar=0,location=0,directories=0,status=0,menubar=0,scrollbars=1,resizable=1,width=490,height=550")
}

function do_getfile(fname,num)
{
window.open("get.php?fileid="+fname,"","toolbar=0,location=0,directories=0,status=0,menubar=0,scrollbars=0,resizable=0,width=400,height=300")
}

function stripFormDefaults(form)
{
    var elementsToEnable = [];
    for (var i = 0; i < form.elements.length; ++i) {
      var element = form.elements[i];
      if (!element.disabled) {
        if (element.type == "text" || element.type == "textarea" ||
            element.type == "file") {
          if (element.value == element.defaultValue) {
            elementsToEnable.push(element);
            element.disabled = true;
          }
        }
        else if (element.type == "hidden") {
          if (element.value == null || element.value.length==0) {
            elementsToEnable.push(element);
            element.disabled = true;
          }
        }
        else if (element.type == "select-one" ||
                   element.type == "select-multiple") {
          var shouldDisable = true;
          // Only disable if all of the options are unchanged.
          for (var j = 0; element.options[j]; ++j) {
            var opt = element.options[j];
            if (opt.selected != opt.defaultSelected) {
              shouldDisable = false;
            }
          }
          if (shouldDisable) {
            elementsToEnable.push(element);
            element.disabled = true;
          }
        } else if (element.type == "checkbox" || element.type == "radio") {
          if (element.checked == element.defaultChecked) {
            elementsToEnable.push(element);
            element.disabled = true;
          }
        }
      }
    }
    form.submit();
    for (var i = 0; i < elementsToEnable.length; ++i) {
      elementsToEnable[i].disabled = false;
    }
    return false;
}



function opensendform()
{
	var text = "";

	text = window.GetSelected().toString();
	$.post(
		"/inc/send_error.php",
		{txt: '' + text, url: '' + window.location.href},
			function(data){
				// create a modal dialog with the data
				$($("<div></div").append(data)).modal({
				overlayId: 'error-overlay',
				containerId: 'error-container',
				position: ["15%",],
				onOpen: senderror.open,
				onShow: senderror.show,
				onClose: senderror.close
				});
			});
}

	var senderror = {
	open: function (dialog) {
		dialog.overlay.fadeIn(200, function () {
			dialog.container.fadeIn(200, function () {
				dialog.data.fadeIn(200);
			});
		});
	},
	show: function (dialog) {
		$('#error-container .send-error-button').click(function (e) {
			var msg = $('#error-container .senderror-message');
			$('#error-container form').fadeOut(200);
			$('#error-container').animate({
			height: '70px'
			}, function () {
					$.ajax({
						url: '/inc/send_error.php',
						data: $('#error-container form').serialize(),
						type: 'post',
						cache: false,
						dataType: 'html',
						success: function (data) {
							msg.html(data).fadeIn(200);
						}
					});
				});
		});
			
	},
	close: function (dialog) {
		$('#error-container form').fadeOut(200);
			dialog.data.fadeOut(200, function () {
				dialog.container.fadeOut(200, function () {
					dialog.overlay.fadeOut(200, function () {
						$.modal.close();
					});
				});
			});
	}
}

function GetSelected()
{
	txt = '';
    if (window.getSelection) {
        txt = window.getSelection();
    } else if (document.getSelection) {
        txt = document.getSelection();
    } else if (document.selection) {
        txt = document.selection.createRange().text;
    }
	return txt;
	return "";
}

$(document).ready( function()
{
	document.onkeypress = function(event) {
		event = (event) ? event : window.event;
		if ((event.keyCode == 13 || event.keyCode == 10) && event.ctrlKey == true && window.GetSelected().toString().length > 1)
		{
			opensendform();
		}
	}
});