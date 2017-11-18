#ifndef DATA_CONSTANTS_H
#define DATA_CONSTANTS_H


enum {
	Advisor_None = 0,
	Advisor_Labor = 1,
	Advisor_Military = 2,
	Advisor_Imperial = 3,
	Advisor_Ratings = 4,
	Advisor_Trade = 5,
	Advisor_Population = 6,
	Advisor_Health = 7,
	Advisor_Education = 8,
	Advisor_Entertainment = 9,
	Advisor_Religion = 10,
	Advisor_Financial = 11,
	Advisor_Chief = 12
};

enum {
	Dir_0_Top = 0,
	Dir_1_TopRight = 1,
	Dir_2_Right = 2,
	Dir_3_BottomRight = 3,
	Dir_4_Bottom = 4,
	Dir_5_BottomLeft = 5,
	Dir_6_Left = 6,
	Dir_7_TopLeft = 7,
	Dir_8_None = 8,
	DirFigure_8_AtDestination = 8,
	DirFigure_9_Reroute = 9,
	DirFigure_10_Lost = 10,
	DirFigure_11_Attack = 11,
};

enum {
	Resource_None = 0,
	Resource_Wheat = 1,
	Resource_Vegetables = 2,
	Resource_Fruit = 3,
	Resource_Olives = 4,
	Resource_Vines = 5,
	Resource_Meat = 6,
	Resource_Wine = 7,
	Resource_Oil = 8,
	Resource_Iron = 9,
	Resource_Timber = 10,
	Resource_Clay = 11,
	Resource_Marble = 12,
	Resource_Weapons = 13,
	Resource_Furniture = 14,
	Resource_Pottery = 15,
	Resource_Denarii = 16,
	Resource_Troops = 17,
	// helper constants
	Resource_Min = 1,
	Resource_Max = 16,
	Resource_MinFood = 1,
	Resource_MaxFood = 7,
};

#define ResourceIsFood(r) ((r) == Resource_Wheat || (r) == Resource_Vegetables || (r) == Resource_Fruit || (r) == Resource_Meat)

enum {
	Inventory_Wheat = 0,
	Inventory_Vegetables = 1,
	Inventory_Fruit = 2,
	Inventory_Meat = 3,
	Inventory_Wine = 4,
	Inventory_Oil = 5,
	Inventory_Furniture = 6,
	Inventory_Pottery = 7,
	// helper constants
	Inventory_MinFood = 0,
	Inventory_MaxFood = 4,
	Inventory_MinGood = 4,
	Inventory_MaxGood = 8,
	Inventory_Max = 8
};

enum {
	WorkshopResource_OlivesToOil = 1,
	WorkshopResource_VinesToWine = 2,
	WorkshopResource_IronToWeapons = 3,
	WorkshopResource_TimberToFurniture = 4,
	WorkshopResource_ClayToPottery = 5
};

enum {
	LaborCategory_IndustryCommerce = 0,
	LaborCategory_FoodProduction = 1,
	LaborCategory_Engineering = 2,
	LaborCategory_Water = 3,
	LaborCategory_Prefectures = 4,
	LaborCategory_Military = 5,
	LaborCategory_Entertainment = 6,
	LaborCategory_HealthEducation = 7,
	LaborCategory_GovernanceReligion = 8
};

enum {
	TradeStatus_None = 0,
	TradeStatus_Import = 1,
	TradeStatus_Export = 2
};

enum {
	EmigrationCause_None = 0,
	EmigrationCause_NoFood = 1,
	EmigrationCause_NoJobs = 2,
	EmigrationCause_HighTaxes = 3,
	EmigrationCause_LowWages = 4,
	EmigrationCause_ManyTents = 5,
};

enum {
	God_Ceres = 0,
	God_Neptune = 1,
	God_Mercury = 2,
	God_Mars = 3,
	God_Venus = 4
};

enum {
	Festival_None = 0,
	Festival_Small = 1,
	Festival_Large = 2,
	Festival_Grand = 3
};

enum {
	EnemyType_0_Barbarian = 0,
	EnemyType_1_Numidian = 1,
	EnemyType_2_Gaul = 2,
	EnemyType_3_Celt = 3,
	EnemyType_4_Goth = 4,
	EnemyType_5_Pergamum = 5,
	EnemyType_6_Seleucid = 6,
	EnemyType_7_Etruscan = 7,
	EnemyType_8_Greek = 8,
	EnemyType_9_Egyptian = 9,
	EnemyType_10_Carthaginian = 10,
	EnemyType_11_Caesar = 11
};

/*
0x00: Barbarians
0x01: Etruscans
0x02: Samnites
0x03: Carthaginians
0x04: Macedonians
0x05: Graeci
0x06: Egyptians
0x07: Pergamum
0x08: Seleucids
0x09: Picts
0x0A: Celts
0x0B: Britons
0x0C: Gauls
0x0D: Helvetii
0x0E: Goths
0x0F: Visigoths
0x10: Huns
0x11: Iberians
0x12: Numidians
0x13: Judaeans
*/
extern const int Constant_SalaryForRank[11];
extern const int Constant_DirectionGridOffsets[8];

struct MissionId {
	int peaceful;
	int military;
};

extern const struct MissionId Constant_MissionIds[12];

#endif
