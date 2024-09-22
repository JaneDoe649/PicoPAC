../../asl-current/asl -L -cpu 8048 text-engine_V31.a48
../../asl-current/asl -L -cpu 8048 text-engine_bank2.a48
../../asl-current/asl -L -cpu 8048 text-engine_bank1.a48
../../asl-current/asl -L -cpu 8048 text-engine_bank0.a48
# cd ..
# del o2em\roms\text-engine_0123.bin
# cd asw
../../asl-current/p2bin text-engine_V31.p text-engine_bank3.bin -r 1024-3071
../../asl-current/p2bin text-engine_bank2.p text-engine_bank2.bin -r 1024-3071
../../asl-current/p2bin text-engine_bank1.p text-engine_bank1.bin -r 1024-3071
../../asl-current/p2bin text-engine_bank0.p text-engine_bank0.bin -r 1024-3071
cat  text-engine_bank0.bin text-engine_bank1.bin text-engine_bank2.bin text-engine_bank3.bin >text-engine_0123.bin
cp text-engine_0123.bin ../selectgame.bin
cp text-engine_0123.bin ../../o2em118src/SRC/roms/selectgame.bin
cd ../../o2em118src/SRC
./o2em -c52 selectgame.bin



