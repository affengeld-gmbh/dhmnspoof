#!
############################################################################
# Copyright (C) 2016  Internet Systems Consortium, Inc. ("ISC")
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
############################################################################

import os
import sys

sys.path.insert(0, os.path.dirname(sys.argv[0]))
if os.name != 'nt':
    sys.path.insert(1, os.path.join('/usr/local', 'lib',
		    'python' + sys.version[:3], 'site-packages'))

import isc.keymgr

if __name__ == "__main__":
    isc.keymgr.main()
