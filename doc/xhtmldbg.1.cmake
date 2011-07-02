.IX Title "XHTMLDBG 1"
.TH XHTMLDBG 1 "2011-06-30" "@XHTMLDBG_VERSION_MAJOR@" "xhtmldbg"
.SH NAME
xhtmldbg \- a HTML/XHTML Debugger and CSS Validator Application
.SH SYNOPSIS
.B xhtmldbg
.RI [ options ] " url" ...
.SH DESCRIPTION
This manual page documents briefly the
.B xhtmldbg 
commands.
.PP
.\" TeX users may be more comfortable with the \fB<whatever>\fP and
.\" \fI<whatever>\fP escape sequences to invode bold face and italics, 
.\" respectively.
\fBxhtmldbg\fP is a XHTML / HTML Debugger for Website Developers.
.PP
All these are good starting points for targeted xhtml development debugging.
.PP
\- to watch HTTP headers,
.br
\- to watch _POST variables when send html forms,
.br
\- css stylesheet watching,
.br
\- dom inspector for better documents structure watching,
.br
\- to indicate css stylesheet attributes,
.br
\- to indicate current used cookies,
.br
\- to watch javascript errors,
.br
\- auto reload function up to 60 seconds,
.br
\- analyzing SEO keywords,
.br
\- quickly HTTP_USER_AGENT switching,
.br
\- strict RFC 2109 (HTTP State Management Mechanism) validation for Cookies,
.br
\- validate ATOM/RSS/RDF Documents,
.br
\- of course, this requires to validate and check the source.
.PP
.B xhtmldbg
is written with Qt4 C++ Cross Platform Framework.
.br
Currently it's only available for Linux but it is envisaged to transport it to all supported platforms by Qt4.
.PP
.B xhtmldbg
contains a webbrowser based on WebKit and provides an ideal area to inspect HTML 4 and 5 Documents.
.PP
.SH OPTIONS
These programs follow the usual GNU command line syntax, with long
options starting with two dashes (`-').
.br
A summary of options is included below.
For a complete description, see the Info files.
.TP
.B \-h, \-\-help
Show summary of options.
.TP
.B \-v, \-\-version
Show version of program.
.SH AUTHOR
xhtmldbg was written by Undefined <nospam(AT)hjcms(DOT)de>.
.SH HOMEPAGE
http://xhtmldbg.hjcms.de
.PP
