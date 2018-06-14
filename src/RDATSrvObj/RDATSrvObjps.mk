
RDATSrvObjps.dll: dlldata.obj RDATSrvObj_p.obj RDATSrvObj_i.obj
	link /dll /out:RDATSrvObjps.dll /def:RDATSrvObjps.def /entry:DllMain dlldata.obj RDATSrvObj_p.obj RDATSrvObj_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del RDATSrvObjps.dll
	@del RDATSrvObjps.lib
	@del RDATSrvObjps.exp
	@del dlldata.obj
	@del RDATSrvObj_p.obj
	@del RDATSrvObj_i.obj
