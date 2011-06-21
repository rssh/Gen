## main definishing part to include in several Makefiles
## $Id: environment.nt.mak,v 1.6 2004-03-13 09:53:19 rssh Exp $

##
# User editable part
##

############################################################
# Uncomment one of these lines regarding to the ORB you have:

ORB=Mico

#ORB=ORBacus

#ORB=OmniORB

#ORB=TAO

########################
# ORB depended settings:

# TAO: set ACE root folder (optionally):

#ACE_ROOT=D:\ORB\ACE_wrappers

#DEBUG_MARK=d

# Set your ORB root folder:

# TAO (optionally):
TAO_ROOT=$(ACE_ROOT)\TAO

# OmniORB:
OMNI_ROOT=D:\ORB\omni

# ORBacus:
OOC_ROOT=D:\Programs\OB-4.1.0

# Mico
MICO_ROOT=c:\home\rssh\packages\mico
PTHREADS_ROOT=c:\home\rssh\packages\pthreads
