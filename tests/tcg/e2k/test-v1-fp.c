#include "test.h"

static uint64_t f32_src1[] = {
    0x447bb0acc44a6472, // 1006.76049805, -809.56945801
    0x4392fdd8c32da2c0, // 293.98315430, -173.63574219
    0x430c0e0043683320, // 140.05468750, 232.19970703
    0xc40743a342938450, // -541.05682373, 73.75842285
    0x40d2e800438bc420, // 6.59082031, 279.53222656
    0x43f8150cc46750f4, // 496.16442871, -925.26489258
    0xc460f441c3f70fe2, // -899.81646729, -494.12408447
    0x43e2efcc446034b8, // 453.87341309, 896.82373047
    0x434b77404483daf6, // 203.46582031, 1054.84252930
    0xc323fc68445d9b94, // -163.98596191, 886.43090820
    0xc3e127e8c487776b, // -450.31176758, -1083.73181152
    0x444bba0cc42bd021, // 814.90698242, -687.25201416
    0xc334a530c350d940, // -180.64526367, -208.84863281
    0xc3b2660cc44b7098, // -356.79724121, -813.75927734
    0x438b5d0c44395f18, // 278.72692871, 741.48583984
    0x43c7cc2443a5daf0, // 399.59484863, 331.71044922
    0xc428a0594400ea10, // -674.50543213, 515.65722656
    0xc4180976c3fb934a, // -608.14782715, -503.15069580
    0xc3d7d246c3d0109a, // -431.64276123, -416.12969971
    0x44810846c4910ff6, // 1032.25854492, -1160.49877930
    0xc2d10cf04418b0e0, // -104.52526855, 610.76367188
    0xc47ecb87c4568dc0, // -1019.18011475, -858.21484375
    0xc46ccd40bf2e2000, // -947.20703125, -0.68017578
    0x444d3614441efc80, // 820.84497070, 635.94531250
    0x433f9d78443cfe7e, // 191.61511230, 755.97644043
    0xc378ecfcc35b0824, // -248.92572021, -219.03179932
    0xc461b317c4828523, // -902.79827881, -1044.16052246
    0x41d3d840438551ec, // 26.48059082, 266.64001465
    0x443a6c80c3f08ba4, // 745.69531250, -481.09094238
    0xc491ac5ec429cd86, // -1165.38647461, -679.21130371
    0x42bfe57043061cd0, // 95.94812012, 134.11254883
    0xc393f0bec480757b, // -295.88079834, -1027.67126465
    0x431ef008c406f8ae, // 158.93762207, -539.88562012
    0xc47d0510c2cda510, // -1012.07910156, -102.82238770
    0x43127ce842d63040, // 146.48791504, 107.09423828
    0x43f4632c445282c8, // 488.77478027, 842.04345703
    0xc490880a445ca838, // -1156.25122070, 882.62841797
    0xc3a02c40c437dcd4, // -320.34570312, -735.45043945
    0x44001b00c43f1345, // 512.42187500, -764.30108643
    0x7fc000007fc00000, // NaN, NaN
};

static uint64_t f32_src2[] = {
    0x4430c1b0c41ea85e, // 707.02636719, -634.63073730
    0xc25f87c0441da702, // -55.88256836, 630.60949707
    0x43d9dfe0c47a613d, // 435.74902344, -1001.51934814
    0x4452fa70c48a4ccf, // 843.91308594, -1106.40026855
    0x44237708c3fdcbd4, // 653.85986328, -507.59240723
    0x44614434c3979980, // 901.06567383, -303.19921875
    0xc462cdf243f5705c, // -907.21789551, 490.87780762
    0xc41497afc3d842ae, // -594.37005615, -432.52093506
    0x435c5558c48d2d82, // 220.33337402, -1129.42211914
    0xc44ddaa2c3b4e9f0, // -823.41613770, -361.82763672
    0xc464230543009808, // -912.54718018, 128.59387207
    0xc43939f14435e174, // -740.90533447, 727.52270508
    0xc48e1d5bc48474f1, // -1136.91735840, -1059.65441895
    0x444f0274436b1f88, // 828.03833008, 235.12316895
    0xc3eddb22443290b4, // -475.71197510, 714.26098633
    0xc45305cac2f65b00, // -844.09045410, -123.17773438
    0x44859cba441e0340, // 1068.89770508, 632.05078125
    0x42b076a0c32f9c20, // 88.23168945, -175.60986328
    0xc40c8e51c3bcc890, // -562.22369385, -377.56689453
    0x447040a4c4403dc0, // 961.01000977, -768.96484375
    0x7fc000007fc00000, // NaN, NaN
    0x445f9868423b6680, // 894.38134766, 46.85009766
    0xc3a936c2c2e9b680, // -338.42779541, -116.85644531
    0xc48e6b4fc2820420, // -1139.35339355, -65.00805664
    0x43efc814c44a2cac, // 479.56311035, -808.69799805
    0x445f74e844018a96, // 893.82666016, 518.16540527
    0x42131e00c42e0792, // 36.77929688, -696.11828613
    0xc466eca240c0b800, // -923.69738770, 6.02246094
    0xc4425b06c2a4f4d0, // -777.42224121, -82.47814941
    0xc38597e4c3d60156, // -267.18664551, -428.01043701
    0xc41406e8c43ac0a2, // -592.10791016, -747.00988770
    0x43c7d55843eeedbc, // 399.66674805, 477.85729980
    0xc3ee01b2448aabfa, // -476.01324463, 1109.37426758
    0x43188dc8c30f5b28, // 152.55383301, -143.35607910
    0xc3104380c3df04a6, // -144.26367188, -446.03631592
    0x43e218c444925f46, // 452.19348145, 1170.97729492
    0x448a4c6644713d00, // 1106.38745117, 964.95312500
    0x4481936844509268, // 1036.60644531, 834.28759766
    0x415d0f80c1d20e40, // 13.81628418, -26.25695801
    0x43c519e843639550, // 394.20239258, 227.58325195
};

static uint64_t f32_src3[] = {
    0x442931dcc4507a68, // 676.77905273, -833.91259766
    0xc4742717c4994200, // -976.61077881, -1226.06250000
    0x44933f94c489a243, // 1177.98681641, -1101.07067871
    0x4495dc5c4352fcb8, // 1198.88623047, 210.98718262
    0x4441683cc3a58252, // 773.62866211, -331.01812744
    0xc39bea4c4482a6e0, // -311.83044434, 1045.21484375
    0xc2357a80448b5290, // -45.36962891, 1114.58007812
    0xc3e1487044907a04, // -450.56591797, 1155.81298828
    0x43aeeb50c453a269, // 349.83837891, -846.53765869
    0xc402daf3c25c47a0, // -523.42108154, -55.06994629
    0x44440bc2c45377b0, // 784.18371582, -845.87011719
    0xc3b8d01ec23d9f80, // -369.62591553, -47.40576172
    0x43f8f4704415856c, // 497.90966797, 598.08471680
    0xc4728d2d43eae130, // -970.20587158, 469.75927734
    0xc4734c244207e0a0, // -973.18969727, 33.96936035
    0x44914010436826b0, // 1162.00195312, 232.15112305
    0xc4111bf2c43fd056, // -580.43664551, -767.25524902
    0x448874b2c25296a0, // 1091.64672852, -52.64709473
    0x4238aee0433409b8, // 46.17077637, 180.03796387
    0x44515d4c443795ba, // 837.45776367, 734.33947754
    0x441c6c8e4443c5d4, // 625.69616699, 783.09106445
    0xc46b0e3ec41d5a12, // -940.22253418, -629.40734863
    0xc413c3ee4489c4a8, // -591.06140137, 1102.14550781
    0x43accfecc41a9703, // 345.62438965, -618.35955811
    0xc3839a8e448cfae2, // -263.20745850, 1127.84008789
    0xc48e895844633be4, // -1140.29199219, 908.93579102
    0xc34192184447f960, // -193.57067871, 799.89648438
    0x7fc000007fc00000, // NaN, NaN
    0x40f8270043ab471c, // 7.75476074, 342.55554199
    0x448d9a40449697e2, // 1132.82031250, 1204.74633789
    0x43122928c48acf92, // 146.16076660, -1110.48657227
    0x434c8130c469c433, // 204.50463867, -935.06561279
    0x43dbea484402d782, // 439.83032227, 523.36730957
    0x44864d60444bd48c, // 1074.41796875, 815.32104492
    0xc31a04104497c740, // -154.01586914, 1214.22656250
    0xc47895cec4996e84, // -994.34069824, -1227.45361328
    0xc48eeca8416a1000, // -1143.39550781, 14.62890625
    0xc43a45954487ea04, // -745.08721924, 1087.31298828
    0xc43fceb744580a5c, // -767.22991943, 864.16186523
    0xc459d3a8c44db9b8, // -871.30712891, -822.90185547
};

static uint64_t f64_src1[] = {
    0x407731940f5e4e3c, // 371.09864747
    0xc08faec8af10aae5, // -1013.84799016
    0x4083f3f83c0425c4, // 638.49620822
    0xc090bae2d6f0175f, // -1070.72152305
    0x4085d14059180ba4, // 698.15641993
    0xc0807b9cad4eb9ef, // -527.45150243
    0xc0759873874b036c, // -345.52820520
    0xc07ff13c7bbc3f9e, // -511.07726644
    0xc0432cffc617de80, // -38.35155560
    0xc0855597b38aa4d5, // -682.69907292
    0x40789de29e61994c, // 393.86782683
    0x4077cf65b9421df8, // 380.96233488
    0xc068a0179196f728, // -197.00287704
    0xc05d907793940e20, // -118.25729837
    0xc06a1d2ced3bdc30, // -208.91173422
    0x4090e1af9ec751fc, // 1080.42150413
    0x405852848817a0e0, // 97.28933909
    0xc082b5c34e41725a, // -598.72036410
    0x408b754713bc990c, // 878.65970561
    0xc090f194d9d045a3, // -1084.39536214
    0xc06decbb1d23aed0, // -239.39784104
    0x407b908430a9cea8, // 441.03227297
    0xc07f766067c6c8de, // -503.39853647
    0x40779dd5a1b4b32c, // 377.86465617
    0xc0829f4eb2b76d1b, // -595.91342681
    0x408e54b2f87878f8, // 970.58738798
    0x7ff8000000000000, // NaN
    0x407ccdf599688364, // 460.87246075
    0x409239a0c34b8b86, // 1166.40699499
    0xc080126044cc5d83, // -514.29700622
    0x407e96f4f0f5c420, // 489.43480011
    0x4091c7d75b6eb362, // 1137.96030972
    0xc0913f3ac524c221, // -1103.80739267
    0x407bf321359f6128, // 447.19560778
    0x40860a9b13f71dda, // 705.32572167
    0x408c688739fadb30, // 909.06602856
    0x40908c1d05e4f24c, // 1059.02834280
    0xc06365de451ce668, // -155.18338256
    0xc08a88cd8d6de1fc, // -849.10036741
    0xc092bebaac39067c, // -1199.68229760
};

static uint64_t f64_src2[] = {
    0xc0807e79e2240705, // -527.80951336
    0x40659e803d4abd90, // 172.95315423
    0xc0833e775c97b31a, // -615.80828208
    0x4071591b4b6106ac, // 277.56916368
    0x409061a63917650c, // 1048.41232716
    0xc08114fed85f3c97, // -546.62443613
    0xc08c44f85be5abe0, // -904.62126903
    0x40930b5b5afee86c, // 1218.83921431
    0x408a47da282038ec, // 840.98152185
    0xc081dcc8e47388cf, // -571.59809199
    0xc0900d1863f121d1, // -1027.27381875
    0x7ff8000000000000, // NaN
    0xc087858841240236, // -752.69153050
    0x4092c4a23f540a46, // 1201.15844470
    0x40868515e995879a, // 720.63569943
    0xc092746c37664797, // -1181.10568008
    0xc0635334ff328dc8, // -154.60021934
    0xc08e9fb38f3f833b, // -979.96267557
    0xc07f6c91e55dc6f8, // -502.78561913
    0xc0816a70aa756120, // -557.30501262
    0xc07ce3d73b1977aa, // -462.24004660
    0x403e1a3f6525fc00, // 30.10252983
    0xc088f5427e577bb6, // -798.65746754
    0x406bd0dc9bb00f18, // 222.52692971
    0x408ff5186eb1eea4, // 1022.63692988
    0xc091bd4015c5e43d, // -1135.31258306
    0xc0833533e113d505, // -614.65033164
    0x4084517821bd5138, // 650.18365810
    0xc07a5e45d6f5f834, // -421.89205071
    0xc06dc7886eb02058, // -238.23540434
    0x40880c1421c4258e, // 769.50983003
    0xc050afc074d23c10, // -66.74612160
    0xc09015f329f0e7bc, // -1029.48746468
    0x4079cf31947a9f1c, // 412.94960449
    0xc07c2bc1af101118, // -450.73478609
    0x40705ee4cd6366c8, // 261.93085994
    0x40906a0225881d96, // 1050.50209630
    0xc08dec1fd5e0c0da, // -957.51554466
    0x407c50b5bd505204, // 453.04437000
    0xc01e3636ea799800, // -7.55294386
};

static uint64_t f64_src3[] = {
    0x408402099481cfd8, // 640.25467779
    0x405b8608e30bfe20, // 110.09429241
    0x4090ab6747099020, // 1066.85085692
    0xc08308796eae3c4e, // -609.05929314
    0xc08b746edf6979d8, // -878.55413706
    0xc08e40eac3b35020, // -968.11463108
    0x4040ba31946d1ce0, // 33.45463806
    0xc07c19c9bfc1dabe, // -449.61175514
    0xc0887d9175b278e7, // -783.69602527
    0x40931c7f3562a32c, // 1223.12422709
    0xc076818c0907b0ce, // -360.09668830
    0x407e96b3208c706c, // 489.41873221
    0xc07982d94014e0a2, // -408.17803963
    0xc07d6cd01d342e7a, // -470.80080910
    0x409073bae5af39f0, // 1052.93251680
    0xc085c6411dd00ae0, // -696.78179514
    0x408de1419062afdc, // 956.15701367
    0xc085091b240df68e, // -673.13825236
    0xc08bf103bf5dd834, // -894.12682985
    0x407729a6d158b358, // 370.60322699
    0x40838791ba67cac6, // 624.94615632
    0x40839a45f4d87882, // 627.28415841
    0x4080971643762d64, // 530.88587086
    0xc0780f96d588047a, // -384.97432473
    0x4089c2df53785b44, // 824.35904593
    0x408ebda8a19b67f4, // 983.70733949
    0x408fbf64cca466ac, // 1015.92421845
    0x40813f3c4b67ac1c, // 551.90444070
    0x408a97d5edb9ab58, // 850.97945733
    0x4087c805dd9ef504, // 761.00286411
    0x4079f7d69215009c, // 415.48988541
    0xc020b2a1728b0880, // -8.34888800
    0xc08bcc5885e977f0, // -889.54322417
    0x408c81266b493ee8, // 912.14375932
    0x408e3b2f5ca74594, // 967.39812594
    0x7ff8000000000000, // NaN
    0xc08e213f3c3634fa, // -964.15587656
    0xc0835c994f33afc7, // -619.57485810
    0x408927b828a80210, // 804.96492130
    0x403b69ee9d9aae40, // 27.41379724
};

static uint64_t f32_cmp_src1[] = {
    0xc490eb9f43e0464c, // -1159.36315918, 448.54919434
    0x42dee4204493ba84, // 111.44555664, 1181.82861328
    0xc43f765e43fe43a8, // -765.84948730, 508.52856445
    0x441f673043c7671c, // 637.61230469, 398.80554199
    0xc4719917446c8ab4, // -966.39202881, 946.16723633
    0xc349d5d8438fc388, // -201.83532715, 287.52758789
    0xc47957dc43cd6bec, // -997.37280273, 410.84313965
    0xc3a89b90444c17ac, // -337.21533203, 816.36987305
    0xc34944d8c40fbfae, // -201.26892090, -574.99499512
    0x7fc000007fc00000, // NaN, NaN
};

static uint64_t f64_cmp_src1[] = {
    0xc0742bfb53deda3e, // -322.74885928
    0xc0794679036ab4cc, // -404.40454427
    0x40517fc435be15c0, // 69.99635070
    0xc0705af773ed2d66, // -261.68541329
    0xc08ec1fe46d066c5, // -984.24915850
    0x4021e3a730ca3800, // 8.94463494
    0x40929c74dcc7e800, // 1191.11412346
    0xc07c9b3cb2d0eeb2, // -457.70231897
    0x4048b8581bef27a0, // 49.44018888
    0x7ff8000000000000, // NaN
};

#ifndef DUMP_ONLY
#include "test-v1-fp-expect.inc"
#endif

static void test_f32(void) {
    CHECK2(EXEC_RR, 0134, fadds, f32_src1, f32_src2);
    CHECK2(EXEC_RR, 0134, fsubs, f32_src1, f32_src2);
    CHECK2(EXEC_RR, 0134, fmins, f32_src1, f32_src2);
    CHECK2(EXEC_RR, 0134, fmaxs, f32_src1, f32_src2);
    CHECK2(EXEC_RR, 0134, fmuls, f32_src1, f32_src2);

    CHECK2_CARTESIAN(EXEC_RR, 0134, fcmpeqs,  f32_cmp_src1, f32_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, fcmplts,  f32_cmp_src1, f32_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, fcmples,  f32_cmp_src1, f32_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, fcmpuods, f32_cmp_src1, f32_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, fcmpneqs, f32_cmp_src1, f32_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, fcmpnlts, f32_cmp_src1, f32_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, fcmpnles, f32_cmp_src1, f32_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, fcmpods,  f32_cmp_src1, f32_cmp_src1);

    CHECK3(EXEC_RRR, 0134, fmul_adds,  f32_src1, f32_src2, f32_src3);
    CHECK3(EXEC_RRR, 0134, fmul_subs,  f32_src1, f32_src2, f32_src3);
    CHECK3(EXEC_RRR, 0134, fmul_rsubs, f32_src1, f32_src2, f32_src3);

    CHECK2_CARTESIAN(EXEC_CMP, 0134, fcmpeqsb,  f32_cmp_src1, f32_cmp_src1);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, fcmpltsb,  f32_cmp_src1, f32_cmp_src1);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, fcmplesb,  f32_cmp_src1, f32_cmp_src1);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, fcmpuodsb, f32_cmp_src1, f32_cmp_src1);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, fcmpneqsb, f32_cmp_src1, f32_cmp_src1);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, fcmpnltsb, f32_cmp_src1, f32_cmp_src1);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, fcmpnlesb, f32_cmp_src1, f32_cmp_src1);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, fcmpodsb,  f32_cmp_src1, f32_cmp_src1);

    // TODO: fdivs
    // TODO: fsqrts
    // TODO: frcps
    // TODO: frsqrts
}

static void test_f64(void) {
    CHECK2(EXEC_RR, 0134, faddd, f64_src1, f64_src2);
    CHECK2(EXEC_RR, 0134, fsubd, f64_src1, f64_src2);
    CHECK2(EXEC_RR, 0134, fmind, f64_src1, f64_src2);
    CHECK2(EXEC_RR, 0134, fmaxd, f64_src1, f64_src2);
    CHECK2(EXEC_RR, 0134, fmuld, f64_src1, f64_src2);

    CHECK2_CARTESIAN(EXEC_RR, 0134, fcmpeqd,  f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, fcmpltd,  f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, fcmpled,  f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, fcmpuodd, f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, fcmpneqd, f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, fcmpnltd, f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, fcmpnled, f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, fcmpodd,  f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, fcmpudsf, f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, fcmpodsf, f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, fcmpuddf, f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, fcmpoddf, f64_cmp_src1, f64_cmp_src1);

    CHECK3(EXEC_RRR, 0134, fmul_addd,  f64_src1, f64_src2, f64_src3);
    CHECK3(EXEC_RRR, 0134, fmul_subd,  f64_src1, f64_src2, f64_src3);
    CHECK3(EXEC_RRR, 0134, fmul_rsubd, f64_src1, f64_src2, f64_src3);

    CHECK2_CARTESIAN(EXEC_CMP, 0134, fcmpeqdb,  f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, fcmpltdb,  f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, fcmpledb,  f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, fcmpuoddb, f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, fcmpneqdb, f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, fcmpnltdb, f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, fcmpnledb, f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, fcmpoddb,  f64_cmp_src1, f64_cmp_src1);

    // TODO: fdivd
    // TODO: fsqrtid
    // TODO: fsqrttd
}

static void test_packed_f32(void) {
    CHECK2(EXEC_RR, 0134, pfadds, f32_src1, f32_src2);
    CHECK2(EXEC_RR, 0134, pfsubs, f32_src1, f32_src2);
    CHECK2(EXEC_RR, 0134, pfmins, f32_src1, f32_src2);
    CHECK2(EXEC_RR, 0134, pfmaxs, f32_src1, f32_src2);
    CHECK2(EXEC_RR, 0134, pfmuls, f32_src1, f32_src2);

    CHECK2_CARTESIAN(EXEC_RR, 0134, pfcmpeqs,  f32_cmp_src1, f32_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, pfcmplts,  f32_cmp_src1, f32_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, pfcmples,  f32_cmp_src1, f32_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, pfcmpuods, f32_cmp_src1, f32_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, pfcmpneqs, f32_cmp_src1, f32_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, pfcmpnlts, f32_cmp_src1, f32_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, pfcmpnles, f32_cmp_src1, f32_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, pfcmpods,  f32_cmp_src1, f32_cmp_src1);

    CHECK3(EXEC_RRR, 0134, pfmul_adds,  f32_src1, f32_src2, f32_src3);
    CHECK3(EXEC_RRR, 0134, pfmul_subs,  f32_src1, f32_src2, f32_src3);
    CHECK3(EXEC_RRR, 0134, pfmul_rsubs, f32_src1, f32_src2, f32_src3);

    // TODO: pfdivs
    // TODO: pfsqrts
}

static void test_packed_f64(void) {
    CHECK2(EXEC_RR, 0134, pfaddd, f64_src1, f64_src2);
    CHECK2(EXEC_RR, 0134, pfsubd, f64_src1, f64_src2);
    CHECK2(EXEC_RR, 0134, pfmind, f64_src1, f64_src2);
    CHECK2(EXEC_RR, 0134, pfmaxd, f64_src1, f64_src2);
    CHECK2(EXEC_RR, 0134, pfmuld, f64_src1, f64_src2);

    CHECK2_CARTESIAN(EXEC_RR, 0134, pfcmpeqd,  f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, pfcmpltd,  f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, pfcmpled,  f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, pfcmpuodd, f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, pfcmpneqd, f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, pfcmpnltd, f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, pfcmpnled, f64_cmp_src1, f64_cmp_src1);
    CHECK2_CARTESIAN(EXEC_RR, 0134, pfcmpodd,  f64_cmp_src1, f64_cmp_src1);

    CHECK3(EXEC_RRR, 0134, pfmul_addd,  f64_src1, f64_src2, f64_src3);
    CHECK3(EXEC_RRR, 0134, pfmul_subd,  f64_src1, f64_src2, f64_src3);
    CHECK3(EXEC_RRR, 0134, pfmul_rsubd, f64_src1, f64_src2, f64_src3);

    // TODO: pfdivd
    // TODO: pfsqrttd
}

static void test_f80(void) {
    // TODO: movfi
    // TODO: movif

    // TODO: fxaddss
    // TODO: fxsubss
    // TODO: fxmulss
    // TODO: fxrsubss
    // TODO: fxdivss
    // TODO: fxdivtss

    // TODO: fxadddd
    // TODO: fxsubdd
    // TODO: fxmuldd
    // TODO: fxrsubdd
    // TODO: fxdivdd
    // TODO: fxdivtdd

    // TODO: fxaddsx
    // TODO: fxsubsx
    // TODO: fxmulsx
    // TODO: fxrsubsx
    // TODO: fxdivsx
    // TODO: fxsqrtisx
    // TODO: fxsqrtusx
    // TODO: fxsqrttsx
    // TODO: fxdivtsx

    // TODO: fxadddx
    // TODO: fxsubdx
    // TODO: fxmuldx
    // TODO: fxrsubdx
    // TODO: fxdivdx
    // TODO: fxdivtdx
    // TODO: fxsqrtidx
    // TODO: fxsqrtudx
    // TODO: fxsqrttdx

    // TODO: fxaddxs
    // TODO: fxsubxs
    // TODO: fxmulxs
    // TODO: fxdivxs

    // TODO: fxaddxd
    // TODO: fxsubxd
    // TODO: fxmulxd
    // TODO: fxdivxd

    // TODO: fxaddxx
    // TODO: fxsubxx
    // TODO: fxmulxx
    // TODO: fxdivxx
    // TODO: fxsqrtixx
    // TODO: fxsqrtuxx
    // TODO: fxsqrttxx

    // TODO: fxcmpeqsb
    // TODO: fxcmpltsb
    // TODO: fxcmplesb
    // TODO: fxcmpuodsb
    // TODO: fxcmpneqsb
    // TODO: fxcmpnltsb
    // TODO: fxcmpnlesb
    // TODO: fxcmpodsb

    // TODO: fxcmpeqdb
    // TODO: fxcmpltdb
    // TODO: fxcmpledb
    // TODO: fxcmpuoddb
    // TODO: fxcmpneqdb
    // TODO: fxcmpnltdb
    // TODO: fxcmpnledb
    // TODO: fxcmpoddb

    // TODO: fxcmpeqxb
    // TODO: fxcmpltxb
    // TODO: fxcmplexb
    // TODO: fxcmpuodxb
    // TODO: fxcmpneqxb
    // TODO: fxcmpnltxb
    // TODO: fxcmpnlexb
    // TODO: fxcmpodxb

    // TODO: fxcmpudsf
    // TODO: fxcmpodsf
    // TODO: fxcmpuddf
    // TODO: fxcmpoddf
    // TODO: fxcmpudxf
    // TODO: fxcmpodxf
}

int main(int argc, char *argv[]) {
    parse_args(argc, argv);

    test_f32();
    test_f64();
    test_packed_f32();
    test_packed_f64();

    test_f80();

// TODO: fstois
// TODO: fstoistr
// TODO: pfstois
// TODO: pfstoistr
// TODO: fstoid
// TODO: istofs
// TODO: fdtoid
// TODO: fxtoid
// TODO: idtofd
// TODO: idtofx
// TODO: fxtofd
// TODO: fdtofx
// TODO: pistofs
// TODO: istofd
// TODO: istofx
// TODO: fstofd
// TODO: fstofx
// TODO: pfstofd
// TODO: fdtois
// TODO: fxtois
// TODO: fdtoistr
// TODO: idtofs
// TODO: fdtofs
// TODO: fxtofs
// TODO: pfdtois
// TODO: pfdtoistr
// TODO: pfdtofs

    // TODO: v1 only, implemented, no support in toolchain
    //
    // fadd_muls
    // fadd_muld
    // fsub_muls
    // fsub_muld
    // pfadd_muls
    // pfadd_muld
    // pfsub_muls
    // pfsub_muld

    return is_failed();
}
