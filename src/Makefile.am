# $Id: Makefile.am 75 2006-06-20 23:38:49Z lennart $
#
# This file is part of PulseAudio Manager.
#
# PulseAudio Manager is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# PulseAudio Manager is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with PulseAudio Manager; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA.

desktopdir = $(datadir)/applications

bin_PROGRAMS=paman3

dist_pkgdata_DATA=paman3.glade
dist_desktop_DATA=paman3.desktop

paman_SOURCES=MainWindow.cc MainWindow.hh \
	SinkWindow.cc SinkWindow.hh \
	SourceWindow.cc SourceWindow.hh \
	ServerInfoManager.cc ServerInfoManager.hh \
	paman.cc paman.hh \
	ClientWindow.cc ClientWindow.hh \
	ModuleWindow.cc ModuleWindow.hh \
	SinkInputWindow.cc SinkInputWindow.hh \
	SourceOutputWindow.cc SourceOutputWindow.hh \
	StatWindow.cc StatWindow.hh \
	SampleWindow.cc SampleWindow.hh

paman_LDADD=$(AM_LDADD) $(GUILIBS_LIBS) $(PULSE_LIBS)
paman_CXXFLAGS=$(AM_CXXFLAGS) $(GUILIBS_CFLAGS) $(PULSE_CFLAGS) 
paman_CXXFLAGS+=-DGLADE_FILE=\"$(pkgdatadir)/paman.glade\"
#paman_CXXFLAGS+=-DPAVUMETER_PATH=\"/home/lennart/projects/pavumeter/src/pavumeter\"
