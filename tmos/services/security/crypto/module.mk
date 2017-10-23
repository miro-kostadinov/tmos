##########################################################################
#
#	Make include for services/security/crypto
#	
#   Platform: MPOS rev.2.3
#
#
##########################################################################

# local sources
local_as_src-y	:=
local_c_src-y 	:=
local_cpp_src-y	:=
local_h_src-y 	:=

local_as_src-y	+= poly1305.S

local_cpp_src-y	+= asn1.cpp base64.cpp chacha.cpp chacha20_poly1305.cpp
local_cpp_src-y	+= cipher_cbc.cpp cipher_ccm.cpp cipher_gcm.cpp crypto.cpp dh.cpp
local_cpp_src-y	+= dsa.cpp ec_curves.cpp ec.cpp ecdh.cpp ecdsa.cpp hmac.cpp
local_cpp_src-y	+= md5.cpp mpi.cpp oid.cpp pem.cpp poly1305.cpp rsa.cpp sha1.cpp
local_cpp_src-y	+= sha224.cpp sha256.cpp sha384.cpp sha512.cpp x509.cpp
local_cpp_src-y	+= rc4.cpp idea.cpp des.cpp des3.cpp aes.cpp camellia.cpp
local_cpp_src-y	+= aria.cpp seed.cpp yarrow.cpp

local_h_src-y 	+= asn1.h base64.h chacha.h chacha20_poly1305.h crypto.h dh.h
local_h_src-y 	+= dsa.h ec_curves.h ec.h ecdh.h ecdsa.h hmac.h md5.h mpi.h 
local_h_src-y 	+= oid.h pem.h poly1305.h rc4.h rsa.h sha1.h sha224.h sha256.h 
local_h_src-y 	+= sha384.h sha512.h x509.h idea.h des.h des3.h aes.h camellia.h
local_h_src-y 	+= aria.h seed.h common.h crypto_opt.h yarrow.h

#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))
h_sources  	+= $(call changepath,$(local_h_src-y))

#add current directory to include path
inc_dirs += $(subdirectory) 

#submodules
local_modules-y := 

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))
