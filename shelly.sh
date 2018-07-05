#!/bin/bash
# This line is a comment.

echo 'BR2_PACKAGE_PRU_USER=y'      >> .config
echo 'BR2_PACKAGE_PRU_CTRL=y'      >> .config
echo 'BR2_PACKAGE_SHBUF=y'         >> .config
echo 'BR2_PACKAGE_SHBUF_USER=y'    >> .config

rm -r output/build/pru_ctrl-1.0/
rm -r output/build/pru_user-1.0/
rm -r output/build/shbuf-1.0/
rm -r output/build/shbuf_user-1.0/

make -j12

