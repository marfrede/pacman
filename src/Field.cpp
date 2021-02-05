#include "Field.h"

Field::Field(int planeWidth, int planeDepth)
{
	this->planeWidth = planeWidth;
	this->planeDepth = planeDepth;
	this->initWallPositions();
	this->initFieldTypesMap();

	pConstShaderPlane = new ConstantShader();
	pPhongShaderWall = new PhongShader();
	pPhongShaderPoint = new PhongShader();

	this->createField();
	this->createWalls();
	this->createPoints();
}

void Field::createField() {

	// CHEQUERED LINE PLAYING FIELD
	int planeWidth = 30, planeDepth = 33;
	pPlane = new LinePlaneModel((float)planeWidth, (float)planeDepth, (float)planeWidth, (float)planeDepth);
	pConstShaderPlane->color(Color(1, 0, 0));
	pPlane->shader(pConstShaderPlane, true);

	// TEXTURED TRIANGLE PLAYING FIELD
 //   pPlane = new TrianglePlaneModel(30, 33, 10, 10);
	//pPhongShader->ambientColor(Color(0.2f,0.2f,0.2f));
	//pPhongShader->diffuseColor(Color(1.0f,1.0f,1.0f));
	//pPhongShader->diffuseTexture(Texture::LoadShared(TEXTURE_DIRECTORY "dirtyBricks_C_01.dds"));
	//pPlane->shader(pPhongShader, true);
}

void Field::createWalls() {
	// 1. set wall padding and height
	float padding = 0.0f;
	float wallHeight = 2.5f;

	// 2. set shader equally for all walls
	pPhongShaderWall = new PhongShader();
	pPhongShaderWall->ambientColor(Color(0.2f, 0.2f, 0.2f)); // normal grey
	//pPhongShaderWall->ambientColor(Color(0.14902f, 0.15294f, 0.8f)); // pacman blue wall color
	pPhongShaderWall->diffuseColor(Color(1.0f, 1.0f, 1.0f));
	pPhongShaderWall->specularColor(Color(1.0f, 1.0f, 1.0f));
	//pPhongShaderWall->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "smiley.png"));
	pPhongShaderWall->diffuseTexture(Texture::LoadShared(TEXTURE_DIRECTORY "PaintedPlaster014_4K_Color.jpg"));

	// 3. make walls
	for (auto const& wall : this->wallPositions)
	{
		Walls.push_back(
			new Wall(this->planeWidth, this->planeDepth,
				wall.second.first, // width
				wallHeight,
				wall.second.second, // depth
				wall.first.first, // posX
				wall.first.second, // posY
				pPhongShaderWall, padding)
		);
	}
}

void Field::createPoints() {

	this->pPhongShaderPoint = new PhongShader();
	this->pPhongShaderPoint->ambientColor(Color(255.0f / 255.0f, 184.0f / 255.0f, 174.0f / 255.0f));
	//this->pPhongShaderPoint->diffuseColor(Color(1.0f, 1.0f, 1.0f));
	//this->pPhongShaderPoint->specularColor(Color(1.0f, 1.0f, 1.0f));
	for (int z = 0; z < this->planeDepth; z++) {
		for (int x = 0; x < this->planeWidth; x++) {
			if (this->fieldTypesMap[z * this->planeWidth + x] == FieldType::Point) {
				Points.push_back(
					new Point(this->planeWidth, this->planeDepth, x, z, 0.12f, this->pPhongShaderPoint)
				);
			}
		}
	}
	/*Points.push_back(
		new Point(this->planeWidth, this->planeDepth, 10, 10, 0.12f, this->pPhongShaderPoint)
	);*/
}

void Field::draw(const Camera camera) {

	this->pPlane->draw(camera);
	for (ModelList::iterator wall = this->Walls.begin(); wall != this->Walls.end(); ++wall)
	{
		(*wall)->draw(camera);
	}
	for (PointList::iterator point = this->Points.begin(); point != this->Points.end(); ++point)
	{
		(*point)->draw(camera);
	}
}

void Field::update(float dtime) {

	for (PointList::iterator point = this->Points.begin(); point != this->Points.end(); ++point)
	{
		(*point)->update(dtime);
	}
}

void Field::end()
{
	delete[] this->fieldTypesMap;
	for (PointList::iterator it = Points.begin(); it != Points.end(); ++it) {
		delete* it;
	}
	this->Points.clear();
	for (ModelList::iterator it = Walls.begin(); it != Walls.end(); ++it) {
		delete* it;
	}
	this->Walls.clear();
	delete this->pPlane;
	delete this->pConstShaderPlane;
	delete this->pPhongShaderWall;
	delete this->pPhongShaderPoint;
}


void Field::initWallPositions() {
	this->wallPositions = {
		// inner walls
		{{3,3}, {4,3}},
		{{8,3}, {5,3}},
		{{17,3}, {5,3}},
		{{23,3}, {4,3}},
		{{3,7}, {4,2}},
		{{8,7}, {2,8}},
		{{11,7}, {8,2}},
		{{20,7}, {2,8}},
		{{23,7}, {4,2}},
		{{14,9}, {2,3}}, // {{14,8}, {2,4}},
		{{10,10}, {3,2}}, // {{9,10}, {4,2}},
		{{17,10}, {3,2}}, // {{17,10}, {4,2}},
		{{11,13}, {3,2}},
		{{16,13}, {3,2}},
		{{11,16}, {8,2}},
		{{11,15}, {2,1}}, // {{10,15}, {2,3}},
		{{17,15}, {2,1}}, // {{16,15}, {2,3}},
		{{8,16}, {2,5}},
		{{20,16}, {2,5}},
		{{11,19}, {8,2}},
		{{14,21}, {2,3}}, // {{14,20}, {2,4}},
		{{3,22}, {4,2}},
		{{8,22}, {5,2}},
		{{17,22}, {5,2}},
		{{23,22}, {4,2}},
		{{5,24}, {2,3}}, // {{5,23}, {2,4}},
		{{23,24}, {2,3}}, // {{23,23}, {2,4}},
		{{8,25}, {2,3}}, // {{8,25}, {2,4}},
		{{11,25}, {8,2}},
		{{20,25}, {2,3}}, // {{20,25}, {2,4}},
		{{14,27}, {2,3}}, // {{14,26}, {2,4}},
		{{3,28}, {10,2}},
		{{17,28}, {10,2}},

		// outer walls
		{{1,1}, {28,1}},
		{{1,2}, {1,8}}, // {{1,1}, {1,10}},
		{{28,2}, {1,8}}, // {{28,1}, {1,10}},
		{{1,10}, {6,1}},
		{{23,10}, {6,1}},
		{{6,11}, {1,3}}, // {{6,10}, {1,5}},
		{{23,11}, {1,3}}, // {{23,10}, {1,5}},
		{{1,14}, {6,1}},
		{{23,14}, {6,1}},
		{{1,16}, {6,1}},
		{{23,16}, {6,1}},
		{{6,17}, {1,3}}, // {{6,16}, {1,5}},
		{{23,17}, {1,3}}, // {{23,16}, {1,5}},
		{{1,20}, {6,1}},
		{{23,20}, {6,1}},
		{{1,21}, {1,10}}, // {{1,20}, {1,12}},
		{{28,21}, {1,10}}, // {{28,20}, {1,12}},
		{{1,31}, {28,1}},
		// outer / inner walls
		{{14,2}, {2,4}},
		{{2,25}, {2,2}},
		{{26,25}, {2,2}},
	};
}

FieldType Field::getFieldType(int x, int z) {

	return this->fieldTypesMap[z * this->planeWidth + x];
}

void Field::initFieldTypesMap() {
	this->fieldTypesMap = new FieldType[this->planeWidth * this->planeDepth];

	// add walls
	// read walls from wallPositions
	for (auto const& wall : wallPositions)
	{
		int width = wall.second.first;
		int depth = wall.second.second;
		int posX = wall.first.first;
		int posZ = wall.first.second;
		for (int x = posX; x < posX + width; x++) {
			for (int z = posZ; z < posZ + depth; z++) {
				this->fieldTypesMap[z * this->planeWidth + x] = FieldType::Wall;
			}
		}
	}

	// set points where no walls - and free where no points
	for (int z = 0; z < this->planeDepth; z++) {
		for (int x = 0; x < this->planeWidth; x++) {
			if (this->fieldTypesMap[z * this->planeWidth + x] != FieldType::Wall) {
				this->fieldTypesMap[z * this->planeWidth + x] = FieldType::Point;
				// no points outside the outer walls
				if (x == 0 || x == 29 || z == 0 || z == 32) {
					this->fieldTypesMap[z * this->planeWidth + x] = FieldType::Free;
				}
				// no points in centrum
				if (x != 7 && x != 22 && z >= 10 && z <= 20) {
					this->fieldTypesMap[z * this->planeWidth + x] = FieldType::Free;
				}
				// no points at this specific position (why Namco?)
				if ((x == 14 || x == 15) && z == 24) {
					this->fieldTypesMap[z * this->planeWidth + x] = FieldType::Free;
				}
			}
		}
	}
	//this->printFieldTypesMap();
	return;
}

void Field::printFieldTypesMap() {
	for (int z = 0; z < this->planeDepth; z++) {
		std::string leading0 = (z >= 10 ? "" : "0");
		std::cout << leading0 << z << " > ";
		for (int x = 0; x < this->planeWidth; x++) {
			if (this->fieldTypesMap[z * this->planeWidth + x] == FieldType::Wall) {
				std::cout << "x";
			}
			else if (this->fieldTypesMap[z * this->planeWidth + x] == FieldType::Point) {
				std::cout << "*";
			}
			else {
				std::cout << "-";
			}
		}
		std::cout << " <" << std::endl;
	}
}