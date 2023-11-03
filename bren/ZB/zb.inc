option expr32
option casemap:none

if BASED_FIXED eq 1
	include zb.fixed.inc
elseif BASED_FLOAT eq 1
	include zb.float.inc
endif