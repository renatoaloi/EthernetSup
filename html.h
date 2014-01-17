/*
  html.h - HTML strings for EthernetSup.
  Created by Renato Aloi, Dec 26, 2013.
  Released into the public domain.
*/

#define P(name)	static const prog_uchar name[] PROGMEM

P(head_ini) = "<!DOCTYPE HTML>"
"<HTML>"
"<HEAD>";

P(head_fim) = "</HEAD>"
"<BODY>";

P(div_ini) = "<div class='super painel'>"
"<p align='center'>Curso Arduino Automation</p>";

P(div_fim) = "<p align='center' class='small'>"
"<a href='#' class='small'>Powered by Serial Link</a></p>"
"</div>"
"</BODY>"
"</HTML>";

P(button_ini) = "<p align='center'><a href='";
P(button_mid1) = "' class='super button ";
P(button_mid2) = "'>";
P(button_fim) = "</a></p>";

P(label_ini) = "<p align='center'><span class='normal painel grey' align='center'>";
P(label_fim) = "</span></p>";

P(dimmer_ini1) = "<p align='center'><span class='normal painel grey' align='center'>";
P(dimmer_ini2) = "<br><a href='";
P(dimmer_mid11) = "' class='super button ";
P(dimmer_mid12) = "'>";
P(dimmer_mid2) = "</a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='";
P(dimmer_mid21) = "' class='super button ";
P(dimmer_mid22) = "'>";
P(dimmer_fim) = "</a></span></p>";

P(dimmer_space) = "&nbsp;";

P(stylesheet) = "<meta name='viewport' content='width=device-width; "
"initial-scale=1.0; maximum-scale=1.0; user-scalable=0;' />" 
"<style media='all' type='text/css' >" 
".painel,.painel:hover,.painel:active,.painel:visited{"
"background:#222;"
"display:inline-block;"
"padding:5px 10px 6px;"
"color:#fff;text-decoration:none;"
"-moz-border-radius:6px;"
"-webkit-border-radius:6px;"
"-moz-box-shadow:0 1px 3px rgba(0,0,0,0.6);"
"-webkit-box-shadow:0 1px 3px rgba(0,0,0,0.6);"
"text-shadow:0 -1px 1px rgba(0,0,0,0.25);"
"border-bottom:1px solid rgba(0,0,0,0.25);"
"position:relative;"
"cursor:default;}"
".grey.painel{background:#EEE;}"
".super.painel{font-size:34px;padding:8px 14px 9px;font-family:arial;}"
".small,.small:hover,.small:active,.small:visited{font-size:11px;"
"text-decoration:none;color: #fff;}"
".normal,.normal:hover,.normal:active,.normal:visited{font-size:16px;"
"text-decoration:none;color: #222;font-family:verdana;}"
".button,.button:visited{background:#222;"
"display:inline-block;padding:5px 10px 6px;color:#fff;text-decoration:none;"
"-moz-border-radius:6px;-webkit-border-radius:6px;"
"-moz-box-shadow:0 1px 3px rgba(0,0,0,0.6);"
"-webkit-box-shadow:0 1px 3px rgba(0,0,0,0.6);"
"text-shadow:0 -1px 1px rgba(0,0,0,0.25);"
"border-bottom:1px solid rgba(0,0,0,0.25);position:relative;cursor:pointer}"
".button:hover{background-color:#111;color:#fff;}"
".button:active{top:1px;}"
".small.button,.small.button:visited{font-size:11px}"
".button,.button:visited,.medium.button,.medium.button:visited{font-size:13px;"
"font-weight:bold;line-height:1;text-shadow:0 -1px 1px rgba(0,0,0,0.25);}"
".large.button,.large.button:visited{font-size:14px;padding:8px 14px 9px;}"
".super.button,.super.button:visited{font-size:34px;padding:8px 14px 9px;}"
".green.button,.green.button:visited{background-color:#91bd09;}"
".green.button:hover{background-color:#749a02;}"
".red.button,.red.button:visited{background-color:#e62727;}"
".red.button:hover{background-color:#cf2525;}"
".blue.button,.blue.button:visited{background-color:#2981e4;}"
".blue.button:hover{background-color:#2575cf;}</style>";