
#include "DetectorConstruction.hh" //! 本文件的源文件

#include "G4RunManager.hh"
#include "G4NistManager.hh"//! 用于访问材料库
#include "G4Box.hh"//! 用于创建长方体
#include "G4Cons.hh"//! 用于创建圆锥体
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//* g4中探测器的设置需要重写Construct函数
G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  //* 建立材料库的访问对象，可以通过该对象使用库里面的所有材料
  //* 类似于Manager类的调用，这里调用了G4NistManager的静态类Instance方法实现对象的创建
  G4NistManager* nist = G4NistManager::Instance();

  // Envelope parameters(外壳参数？)
  // g4的基本数据类型可以带单位
  G4double env_sizeXY = 20*cm, env_sizeZ = 30*cm;
  //* 通过材料库来创建一个的材料对象
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");

  // Option to switch on/off checking of volumes overlaps（一个用于检查是否有体积重叠的bool）
  //
  G4bool checkOverlaps = true;

  //
  // todo: 1.World
  // todo:  世界world就是创建一个能够容纳所有内容的空间
  //
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  //* 使用G4Box创建一个长方体的空间
  G4Box* solidWorld =
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
  //* 我们创建的这个长方体空间用什么材料填充，这需要用逻辑体去控制
  G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,          //its solid：链接对应的空间（实体）
                        world_mat,           //its material：对应空间的填充材料
                        "World");            //*its name：这里的name只是一种标识，只是为了让用户迅速分辨实体，逻辑体
                                             //* 和物理体三者是一体的
  //* 创建物理体，通过链接逻辑体控制实体的位置，检查是否进行组合等
  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation：不进行旋转
                      G4ThreeVector(),       //at (0,0,0)：需要定义一个G4向量去控制这个实体的位置
                                             //* 这里相当于：G4ThreeVector g4=G4ThreeVector();该方法直接抛出一个
                                             //* G4ThreeVector普通对象作为参数，而不用重新定义一个对象，这样实现一次性用法
                      logicWorld,            //its logical volume：链接对应的逻辑体
                      "World",               //its name
                      0,                     //its mother  volume：当这个逻辑体是想代表World的时候，设置为0
                      false,                 //no boolean operation：不是组合体，不需要做图形运算
                      0,                     //copy number ：不复制
                      checkOverlaps);        //overlaps checking：检查是否有重叠

  //
  // Envelope
  // todo: 2.创建一个空间，用于容纳我们的探测器
  //* 在这里不仅需要创建探测器的形状，也要创建探测器周围的环境
  //* 当然本案例简单得认为容纳探测器的空间就是探测器的形状
  G4Box* solidEnv =
    new G4Box("Envelope",                    //its name
        0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size

  G4LogicalVolume* logicEnv =
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name

  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //* at (0,0,0)：这里的位置的意思是，子体的中心在以母体中心为轴的空间中对应的坐标点
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //* its mother  volume：设置母体对应的逻辑体
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  //
  // Shape 1
  // todo:3.创建的真正的“障碍物？？”
  G4Material* shape1_mat = nist->FindOrBuildMaterial("G4_A-150_TISSUE");
  G4ThreeVector pos1 = G4ThreeVector(0, 2*cm, -7*cm);

  // Conical section shape
  G4double shape1_rmina =  0.*cm, shape1_rmaxa = 2.*cm;
  G4double shape1_rminb =  0.*cm, shape1_rmaxb = 4.*cm;
  G4double shape1_hz = 3.*cm;
  G4double shape1_phimin = 0.*deg, shape1_phimax = 360.*deg;
  //*
  G4Cons* solidShape1 =
    new G4Cons("Shape1",
    shape1_rmina, shape1_rmaxa, shape1_rminb, shape1_rmaxb, shape1_hz,
    shape1_phimin, shape1_phimax);

  G4LogicalVolume* logicShape1 =
    new G4LogicalVolume(solidShape1,         //its solid
                        shape1_mat,          //its material
                        "Shape1");           //its name

  new G4PVPlacement(0,                       //no rotation
                    pos1,                    //at position
                    logicShape1,             //its logical volume
                    "Shape1",                //its name
                    logicEnv,                //*its mother  volume：障碍物拿探测器当参照物一般是最好处理的
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  //
  // Shape 2
  //
  G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");
  G4ThreeVector pos2 = G4ThreeVector(0, -1*cm, 7*cm);

  // Trapezoid shape
  G4double shape2_dxa = 12*cm, shape2_dxb = 12*cm;
  G4double shape2_dya = 10*cm, shape2_dyb = 16*cm;
  G4double shape2_dz  = 6*cm;
  G4Trd* solidShape2 =
    new G4Trd("Shape2",                      //its name
              0.5*shape2_dxa, 0.5*shape2_dxb,
              0.5*shape2_dya, 0.5*shape2_dyb, 0.5*shape2_dz); //its size

  G4LogicalVolume* logicShape2 =
    new G4LogicalVolume(solidShape2,         //its solid
                        shape2_mat,          //its material
                        "Shape2");           //its name

  new G4PVPlacement(0,                       //no rotation
                    pos2,                    //at position
                    logicShape2,             //its logical volume
                    "Shape2",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  // Set Shape2 as scoring volume
  //* 将逻辑体设置为我感兴趣的区域，这样g4就会自动记录这个区域的信息（能量沉积或者径迹或者在这里发生的某种相互作用的相关信息，这和探测器不一样，这是直接跟踪获取而不是探测得到的）
  fScoringVolume = logicShape2;

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
