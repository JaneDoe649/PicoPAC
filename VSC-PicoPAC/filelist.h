/*
//                       PicoPAC MultiCART by Andrea Ottaviani 2024
//
//  VIDEOPAC  multicart based on Raspberry Pico board -
//
//  More info on https://github.com/aotta/ 
//
//   parts of code are directly from the A8PicoCart project by Robin Edwards 2023
//  
//   Needs to be a release NOT debug build for the cartridge emulation to work
// 
//   Edit myboard.h depending on the type of flash memory on the pico clone//
//
//   v. 1.0 2024-08-05 : Initial version for Pi Pico 
//
*/

char gamelist[255][32]= {
// 12345678901234567890123456789012  
  "60 PLUS -TRANS.BIN", // TO CORRECT
  "BR-16.BIN",
  "BR-17.BIN",
  "BR-19.BIN",
  "BR-21.BIN",
  "BR-46.BIN",
  "BR-50.BIN",
  "BR-54.BIN",
  "BR-55.BIN",
  "BR-58.BIN",
  "BR-CLAY-PIGEON.BIN",
  "BR-COMMANDO-NOTURNO.BIN",
  "br_frogger.bin",
  "br_popeye.bin",
  "br_super-cobra.bin",
  "csv1.bin",
  "csv2.bin",
  "im_atlantis.bin",
  "im_demon-attack.bin",
  "jo_basket-bowling_pl.bin",
  "jo_billard_pl.bin",
"jo_chez-maxime.bin",
"jo_demon-attack_pl.bin",
"jo_exojet_pl.bin",
"jo_flipper_pl.bin",
"jo_le-tresor-englouti_pl.bin",
"jo_moto-crash_pl.bin",
"jo_syracuse.bin",
"LISTFILES.TXT",
"mod_01pl.bin",
"mod_05_g7400.bin",
"mod_06pl.bin",
"mod_11pl.bin",
"mod_14fix.bin",
"mod_19_g7400.bin",
"mod_20pl.bin",
"mod_24pl.bin",
"mod_28fix.bin",
"mod_30fix.bin",
"mod_31_g7400.bin",
"mod_35pl_fix.bin",
"mod_36fix.bin",
"mod_40_g7400.bin",
"mod_43pl.bin",
"mod_55pl.bin",
"mod_br21_fix.bin",
"mod_demon-attack_pl.bin",
"mod_moto-crash_g7000.bin",
"mod_playtag_fix.bin",
"mod_tutankham_fix.bin",
"mod_vp31_examples.bin",
"mod_vp9_examples.bin",
"new_amok.bin",
"new_amok_alt.bin",
"new_calculator.bin",
"new_helicopter_buzzword.bin",
"new_jg-munchkin.bin",
"new_kc-pacman.bin",
"new_kc-pacvid.bin",
"new_ktaa-demo1.bin",
"new_ktaa-demo2.bin",
"new_ktaa.bin",
"new_mrroboto.bin",
"new_planet-lander.bin",
"new_pong.bin",
"new_pong_all.bin",
"new_ppp-o2em.bin",
"new_puzzle-piece-panic.bin",
"new_route66.bin",
"ntsc_57.bin",
"ntsc_chez-maxime.bin",
"ntsc_exojet_pl.bin",
"ntsc_interpol.bin",
"ntsc_moto-crash.bin",
"ntsc_shark-hunter.bin",
"ntsc_tutankham.bin",
"o2_06.bin",
"o2_07.bin",
"o2_10.bin",
"o2_11.bin",
"o2_14.bin",
"o2_16.bin",
"o2_19.bin",
"o2_21.bin",
"o2_30.bin",
"o2_33.bin",
"o2_35.bin",
"o2_38.bin",
"o2_39.bin",
"o2_40.bin",
"o2_41.bin",
"o2_43.bin",
"o2_45.bin",
"o2_46.bin",
"o2_47.bin",
"o2_48.bin",
"o2_48alt.bin",
"pal_acrobats.bin",
"pal_flashpoint.bin",
"pal_nimble-numbers-ned.bin",
"pal_type-and-tell.bin",
"pb_frogger.bin",
"pb_popeye.bin",
"pb_q-bert.bin",
"pb_super-cobra.bin",
"pr_clay-pigeon_pl.bin",
"pr_clay-pigeon_pl_alt.bin",
"pr_interpol.bin",
"pr_martian-threat.bin",
"pr_martian-threat_alt.bin",
"pr_mission-impossible.bin",
"pr_nightfighter.bin",
"pr_pinball.bin",
"pr_playtag.bin",
"pr_red-baron.bin",
"pr_red-baron_alt.bin",
"pr_robot-city.bin",
"pr_shark-hunter.bin",
"pr_spiderman.bin",
"pr_spiderman_alt.bin",
"pr_tutankham.bin",
"vp01+_F.bin",
"Vp01_F.bin",
"Vp05_F.bin",
"Vp06_F.bin",
"Vp07_F.bin",
"Vp11+_F.bin",
"Vp11_F.bin",
"Vp12_F.bin",
"Vp18_F.bin",
"Vp23_F.bin",
"Vp24_F.bin",
"Vp25_F.bin",
"Vp28_F.bin",
"Vp29_F.bin",
"Vp35_F.bin",
"Vp36_F.bin",
"Vp40_F.bin",
"Vp42_F.bin",
"Vp45_F.bin",
"Vp47_F.bin",
"Vp52+_F.bin",
"vp_01.bin",
"vp_01hack.bin",
"vp_01pl.bin",
"vp_02.bin",
"vp_03.bin",
"vp_04.bin",
"vp_05.bin",
"vp_06.bin",
"vp_06pl.bin",
"vp_07.bin",
"vp_08.bin",
"vp_09.bin",
"vp_10.bin",
"vp_11.bin",
"vp_11alt.bin",
"vp_11pl.bin",
"vp_12.bin",
"vp_13.bin",
"vp_14.bin",
"vp_15.bin",
"vp_16.bin",
"vp_17.bin",
"vp_18.bin",
"vp_19.bin",
"vp_20.bin",
"vp_20pl.bin",
"vp_21.bin",
"vp_22.bin",
"vp_22hack.bin",
"vp_23.bin",
"vp_24.bin",
"vp_24pl.bin",
"vp_25.bin",
"vp_26.bin",
"vp_27.bin",
"vp_28.bin",
"vp_29.bin",
"vp_30.bin",
"vp_31.bin",
"vp_32.bin",
"vp_33.bin",
"vp_33alt.bin",
"vp_34.bin",
"vp_34pl.bin",
"vp_35.bin",
"vp_35pl.bin",
"vp_36.bin",
"vp_37.bin",
"vp_38.bin",
"vp_39.bin",
"vp_39pl.bin",
"vp_40.bin",
"vp_41.bin",
"vp_42.bin",
"vp_43.bin",
"vp_43pl.bin",
"vp_44.bin",
"vp_45.bin",
"vp_46.bin",
"vp_47.bin",
"vp_48.bin",
"vp_49.bin",
"vp_50.bin",
"vp_51pl.bin",
"vp_52pl.bin",
"vp_53.bin",
"vp_53pl.bin",
"vp_54.bin",
"vp_54pl.bin",
"vp_55.bin",
"vp_55_12.bin",
"vp_55_12fix.bin",
"vp_56pl.bin",
"vp_57.bin",
"vp_58_12.bin",
"vp_59_16.bin",
"vp_60_16.bin",
"vp_a.bin",
"vp_C7010.bin",
"vp_v.bin"
}
