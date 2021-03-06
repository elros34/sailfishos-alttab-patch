
Name:       sailfishos-alttab-patch

# >> macros
# << macros

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}
Summary:    My Sailfish OS Application
Version:    0.0.2
Release:    1
Group:      Qt/Qt
License:    LICENSE
URL:        http://example.org/
Source0:    %{name}-%{version}.tar.bz2
Requires:   sailfish-version >= 3.0.2
Requires:   patchmanager
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)

%description
Short description of my Sailfish OS Application


%prep
%setup -q -n %{name}-%{version}


%build
%qtc_qmake5 

%qtc_make %{?_smp_mflags}


%install
rm -rf %{buildroot}
%qmake5_install

mkdir -p %{buildroot}/usr/share/patchmanager/patches/%{name}
cp -r patch/* %{buildroot}/usr/share/patchmanager/patches/%{name}

%pre
if [ -d /var/lib/patchmanager/ausmt/patches/%{name} ]; then
/usr/sbin/patchmanager -u %{name} || true
fi

%preun
if [ -d /var/lib/patchmanager/ausmt/patches/%{name} ]; then
/usr/sbin/patchmanager -u %{name} || true
fi


%files
%defattr(644,root,root,-)
%{_libdir}/qt5/qml/com/alttabhandler/libalttab-handler.so
%{_libdir}/qt5/qml/com/alttabhandler/qmldir
%{_datadir}/patchmanager/patches/%{name}

