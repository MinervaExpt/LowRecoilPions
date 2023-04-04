#! /usr/bin/env python


from __future__ import absolute_import
if __name__ == '__main__':
    try:
        import os,sys
        # fix up path to find the code
        ourpath = sys.path[0]
        sys.path.insert(0, os.path.normpath(os.path.join(ourpath, '../python')))

        import samweb_cli

        sys.exit(samweb_cli.main())
    except KeyboardInterrupt:
        sys.exit(130)

