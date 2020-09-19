#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer Dx11ConstantBuffer
// {
//
//   float4x4 MODEL_TRANSFORM;          // Offset:    0 Size:    64 [unused]
//   float4x4 VIEW_TRANSFORM;           // Offset:   64 Size:    64 [unused]
//   float4x4 PROJECTION_TRANSFORM;     // Offset:  128 Size:    64 [unused]
//   float4x4 VIEW_PROJECTION_TRANSFORM;// Offset:  192 Size:    64 [unused]
//   float4x4 MODEL_VIEW_PROJECTION_TRANSFORM;// Offset:  256 Size:    64 [unused]
//   float3 CAMERA_POSITION;            // Offset:  320 Size:    12 [unused]
//   float4 MODEL_COLOR_MULTIPLIER;     // Offset:  336 Size:    16
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// Dx11ConstantBuffer                cbuffer      NA          NA            cb0      1 
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float       
// NORMAL                   0   xyz         1     NONE   float   xyz 
// PSIZE                    1      w        1     NONE   float      w
// TEXCOORD                 0   xy          2     NONE   float       
// PSIZE                    0     z         2     NONE   float     z 
// COLOR                    0   xyz         3     NONE   float   xyz 
// COLOR                    1   xyz         4     NONE   float   xyz 
// COLOR                    2   xyz         5     NONE   float   xyz 
// NORMAL                   1   xyz         6     NONE   float   xyz 
// NORMAL                   2   xyz         7     NONE   float   xyz 
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_TARGET                0   xyzw        0   TARGET   float   xyzw
//
ps_4_0
dcl_constantbuffer CB0[22], immediateIndexed
dcl_input_ps linear v1.xyz
dcl_input_ps linear v1.w
dcl_input_ps linear v2.z
dcl_input_ps linear v3.xyz
dcl_input_ps linear v4.xyz
dcl_input_ps linear v5.xyz
dcl_input_ps linear v6.xyz
dcl_input_ps linear v7.xyz
dcl_output o0.xyzw
dcl_temps 2
dp3 r0.x, v7.xyzx, v7.xyzx
rsq r0.x, r0.x
mul r0.xyz, r0.xxxx, v7.xyzx
dp3 r0.w, v6.xyzx, v6.xyzx
rsq r0.w, r0.w
mul r1.xyz, r0.wwww, v6.xyzx
dp3 r0.x, r1.xyzx, r0.xyzx
max r0.x, r0.x, l(0.000000)
log r0.x, r0.x
mul r0.x, r0.x, v2.z
exp r0.x, r0.x
min r0.x, r0.x, l(1.000000)
dp3 r0.y, v1.xyzx, v1.xyzx
rsq r0.y, r0.y
mul r0.yzw, r0.yyyy, v1.xxyz
dp3 r0.y, r0.yzwy, r1.xyzx
mov_sat r0.z, v1.w
mul r0.y, r0.y, r0.z
mul r1.xyz, v3.xyzx, l(0.250000, 0.250000, 0.250000, 0.000000)
mad r0.yzw, v4.xxyz, r0.yyyy, r1.xxyz
mad r0.xyz, v5.xyzx, r0.xxxx, r0.yzwy
mov r0.w, l(1.000000)
mul o0.xyzw, r0.xyzw, cb0[21].xyzw
ret 
// Approximately 24 instruction slots used
#endif

const BYTE G_pixel_shader[] =
{
     68,  88,  66,  67, 246, 112, 
      7, 162,   1, 236,  44, 148, 
    224, 234,  82,  90,  57, 103, 
    163, 220,   1,   0,   0,   0, 
     24,   7,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
     60,   2,   0,   0, 100,   3, 
      0,   0, 152,   3,   0,   0, 
    156,   6,   0,   0,  82,  68, 
     69,  70,   0,   2,   0,   0, 
      1,   0,   0,   0,  80,   0, 
      0,   0,   1,   0,   0,   0, 
     28,   0,   0,   0,   0,   4, 
    255, 255,   0,   1,   0,   0, 
    216,   1,   0,   0,  60,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     68, 120,  49,  49,  67, 111, 
    110, 115, 116,  97, 110, 116, 
     66, 117, 102, 102, 101, 114, 
      0, 171,  60,   0,   0,   0, 
      7,   0,   0,   0, 104,   0, 
      0,   0,  96,   1,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  16,   1,   0,   0, 
      0,   0,   0,   0,  64,   0, 
      0,   0,   0,   0,   0,   0, 
     32,   1,   0,   0,   0,   0, 
      0,   0,  48,   1,   0,   0, 
     64,   0,   0,   0,  64,   0, 
      0,   0,   0,   0,   0,   0, 
     32,   1,   0,   0,   0,   0, 
      0,   0,  63,   1,   0,   0, 
    128,   0,   0,   0,  64,   0, 
      0,   0,   0,   0,   0,   0, 
     32,   1,   0,   0,   0,   0, 
      0,   0,  84,   1,   0,   0, 
    192,   0,   0,   0,  64,   0, 
      0,   0,   0,   0,   0,   0, 
     32,   1,   0,   0,   0,   0, 
      0,   0, 110,   1,   0,   0, 
      0,   1,   0,   0,  64,   0, 
      0,   0,   0,   0,   0,   0, 
     32,   1,   0,   0,   0,   0, 
      0,   0, 142,   1,   0,   0, 
     64,   1,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    160,   1,   0,   0,   0,   0, 
      0,   0, 176,   1,   0,   0, 
     80,   1,   0,   0,  16,   0, 
      0,   0,   2,   0,   0,   0, 
    200,   1,   0,   0,   0,   0, 
      0,   0,  77,  79,  68,  69, 
     76,  95,  84,  82,  65,  78, 
     83,  70,  79,  82,  77,   0, 
      3,   0,   3,   0,   4,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  86,  73, 
     69,  87,  95,  84,  82,  65, 
     78,  83,  70,  79,  82,  77, 
      0,  80,  82,  79,  74,  69, 
     67,  84,  73,  79,  78,  95, 
     84,  82,  65,  78,  83,  70, 
     79,  82,  77,   0,  86,  73, 
     69,  87,  95,  80,  82,  79, 
     74,  69,  67,  84,  73,  79, 
     78,  95,  84,  82,  65,  78, 
     83,  70,  79,  82,  77,   0, 
     77,  79,  68,  69,  76,  95, 
     86,  73,  69,  87,  95,  80, 
     82,  79,  74,  69,  67,  84, 
     73,  79,  78,  95,  84,  82, 
     65,  78,  83,  70,  79,  82, 
     77,   0,  67,  65,  77,  69, 
     82,  65,  95,  80,  79,  83, 
     73,  84,  73,  79,  78,   0, 
    171, 171,   1,   0,   3,   0, 
      1,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     77,  79,  68,  69,  76,  95, 
     67,  79,  76,  79,  82,  95, 
     77,  85,  76,  84,  73,  80, 
     76,  73,  69,  82,   0, 171, 
      1,   0,   3,   0,   1,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  77, 105, 
     99, 114, 111, 115, 111, 102, 
    116,  32,  40,  82,  41,  32, 
     72,  76,  83,  76,  32,  83, 
    104,  97, 100, 101, 114,  32, 
     67, 111, 109, 112, 105, 108, 
    101, 114,  32,  49,  48,  46, 
     49,   0,  73,  83,  71,  78, 
     32,   1,   0,   0,  10,   0, 
      0,   0,   8,   0,   0,   0, 
    248,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
      4,   1,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   7,   7,   0,   0, 
     11,   1,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   8,   8,   0,   0, 
     17,   1,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,   3,   0,   0,   0, 
     11,   1,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,   4,   4,   0,   0, 
     26,   1,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   3,   0, 
      0,   0,   7,   7,   0,   0, 
     26,   1,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   4,   0, 
      0,   0,   7,   7,   0,   0, 
     26,   1,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   5,   0, 
      0,   0,   7,   7,   0,   0, 
      4,   1,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   6,   0, 
      0,   0,   7,   7,   0,   0, 
      4,   1,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   7,   0, 
      0,   0,   7,   7,   0,   0, 
     83,  86,  95,  80,  79,  83, 
     73,  84,  73,  79,  78,   0, 
     78,  79,  82,  77,  65,  76, 
      0,  80,  83,  73,  90,  69, 
      0,  84,  69,  88,  67,  79, 
     79,  82,  68,   0,  67,  79, 
     76,  79,  82,   0,  79,  83, 
     71,  78,  44,   0,   0,   0, 
      1,   0,   0,   0,   8,   0, 
      0,   0,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  15,   0, 
      0,   0,  83,  86,  95,  84, 
     65,  82,  71,  69,  84,   0, 
    171, 171,  83,  72,  68,  82, 
    252,   2,   0,   0,  64,   0, 
      0,   0, 191,   0,   0,   0, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
     22,   0,   0,   0,  98,  16, 
      0,   3, 114,  16,  16,   0, 
      1,   0,   0,   0,  98,  16, 
      0,   3, 130,  16,  16,   0, 
      1,   0,   0,   0,  98,  16, 
      0,   3,  66,  16,  16,   0, 
      2,   0,   0,   0,  98,  16, 
      0,   3, 114,  16,  16,   0, 
      3,   0,   0,   0,  98,  16, 
      0,   3, 114,  16,  16,   0, 
      4,   0,   0,   0,  98,  16, 
      0,   3, 114,  16,  16,   0, 
      5,   0,   0,   0,  98,  16, 
      0,   3, 114,  16,  16,   0, 
      6,   0,   0,   0,  98,  16, 
      0,   3, 114,  16,  16,   0, 
      7,   0,   0,   0, 101,   0, 
      0,   3, 242,  32,  16,   0, 
      0,   0,   0,   0, 104,   0, 
      0,   2,   2,   0,   0,   0, 
     16,   0,   0,   7,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  18,  16,   0,   7,   0, 
      0,   0,  70,  18,  16,   0, 
      7,   0,   0,   0,  68,   0, 
      0,   5,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   7, 114,   0, 
     16,   0,   0,   0,   0,   0, 
      6,   0,  16,   0,   0,   0, 
      0,   0,  70,  18,  16,   0, 
      7,   0,   0,   0,  16,   0, 
      0,   7, 130,   0,  16,   0, 
      0,   0,   0,   0,  70,  18, 
     16,   0,   6,   0,   0,   0, 
     70,  18,  16,   0,   6,   0, 
      0,   0,  68,   0,   0,   5, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7, 114,   0,  16,   0, 
      1,   0,   0,   0, 246,  15, 
     16,   0,   0,   0,   0,   0, 
     70,  18,  16,   0,   6,   0, 
      0,   0,  16,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     52,   0,   0,   7,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  47,   0, 
      0,   5,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   7,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  42,  16,  16,   0, 
      2,   0,   0,   0,  25,   0, 
      0,   5,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     51,   0,   0,   7,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  63,  16,   0, 
      0,   7,  34,   0,  16,   0, 
      0,   0,   0,   0,  70,  18, 
     16,   0,   1,   0,   0,   0, 
     70,  18,  16,   0,   1,   0, 
      0,   0,  68,   0,   0,   5, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7, 226,   0,  16,   0, 
      0,   0,   0,   0,  86,   5, 
     16,   0,   0,   0,   0,   0, 
      6,  25,  16,   0,   1,   0, 
      0,   0,  16,   0,   0,   7, 
     34,   0,  16,   0,   0,   0, 
      0,   0, 150,   7,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     54,  32,   0,   5,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     58,  16,  16,   0,   1,   0, 
      0,   0,  56,   0,   0,   7, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,  10, 114,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  18,  16,   0,   3,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0, 128,  62,   0,   0, 
    128,  62,   0,   0, 128,  62, 
      0,   0,   0,   0,  50,   0, 
      0,   9, 226,   0,  16,   0, 
      0,   0,   0,   0,   6,  25, 
     16,   0,   4,   0,   0,   0, 
     86,   5,  16,   0,   0,   0, 
      0,   0,   6,   9,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,   9, 114,   0,  16,   0, 
      0,   0,   0,   0,  70,  18, 
     16,   0,   5,   0,   0,   0, 
      6,   0,  16,   0,   0,   0, 
      0,   0, 150,   7,  16,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   5, 130,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  63, 
     56,   0,   0,   8, 242,  32, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,  21,   0, 
      0,   0,  62,   0,   0,   1, 
     83,  84,  65,  84, 116,   0, 
      0,   0,  24,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   9,   0,   0,   0, 
     21,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0
};
