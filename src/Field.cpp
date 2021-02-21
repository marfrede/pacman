#include "Field.h"

Field::Field(float wallHeight)
{
	this->initWallPositions();
	this->initFieldTypesMap();

	this->pShaderPortal = new PhongShader();
	this->pShaderWall = new PhongShader();
	this->pShaderPoint = new ConstantShader();
    
    

	this->createField();
	this->createWalls(wallHeight);
	if (SHOW_PORTALS) this->createPortals();
    if (SHOW_POINTS) {
        this->pointCol = Color(255.0f / 255.0f, 184.0f / 255.0f, 174.0f / 255.0f);
        this->createPoints();
        this->particlePopEmitter = new ParticlePopEmitter(100, pointCol);
    }
}

Field::~Field() {
	this->end();
}

void Field::reset() {
	delete this->fieldTypesMap;
	this->initFieldTypesMap();
	if (SHOW_POINTS) this->createPoints();
	return;
}

void Field::createField() {
	int planeWidth = 30, planeDepth = 33;

	if (SHOW_DEBUG_PLANE) {
		// RED CHEQUERED LINE PLAYING FIELD
		ConstantShader* pConstShader = new ConstantShader();
		pPlaneDebug = new LinePlaneModel((float)planeWidth, (float)planeDepth, (float)planeWidth, (float)planeDepth, 0.01f);
		pConstShader->color(Color(1, 0, 0));
		pPlaneDebug->shader(pConstShader, true);
	}
	if (SHOW_PLANE) {
		// TEXTURED TRIANGLE PLAYING FIELD
		//pPlane = new TrianglePlaneModel((float)planeWidth, (float)planeDepth, (float)planeWidth / 4.0f, (float)planeDepth / 4.0f);
		pPlane = new TriangleBoxModel(30, 0.0f, 33);
		PhongShader* pPhongShader = new PhongShader();
		pPhongShader->ambientColor(Color(0.2f, 0.2f, 0.2f));
		pPhongShader->diffuseColor(Color(1.0f, 1.0f, 1.0f));
		pPhongShader->diffuseTexture(Texture::LoadShared(TEXTURE_DIRECTORY "MetalPlates004_1K_Color.jpg"));
		pPhongShader->normalTexture(Texture::LoadShared(TEXTURE_DIRECTORY "MetalPlates004_1K_Normal.jpg"));
		pPlane->shader(pPhongShader, true);
	}
}

void Field::createWalls(float wallHeight) {
	// 2. make shader
	this->pShaderWall->ambientColor(Color(0.2f, 0.2f, 0.2f)); // grey
	this->pShaderWall->diffuseColor(Color(1.0f, 1.0f, 1.0f));
	this->pShaderWall->specularColor(Color(1.0f, 1.0f, 1.0f));
	this->pShaderWall->diffuseTexture(Texture::LoadShared(TEXTURE_DIRECTORY "MetalPlates004_1K_Color.jpg"));
	this->pShaderWall->normalTexture(Texture::LoadShared(TEXTURE_DIRECTORY "MetalPlates004_1K_Normal.jpg"));

	// 2. make walls
	for (auto const& wall : this->wallPositions)
	{
		Walls.push_back(
			new Wall(
				wall.second.first, // width
				wallHeight,
				wall.second.second, // depth
				wall.first.first, // posX
				wall.first.second, // posY
				this->pShaderWall,
				WALL_PADDING
			)
		);
	}
}

/**
* adds the missing points. Initial all points and for further games only the ones missing.
*/
void Field::createPoints() {
	this->pShaderPoint->color(this->pointCol);
	for (int z = 0; z < PLANE_DEPTH; z++) {
		for (int x = 0; x < PLANE_WIDTH; x++) {
			if (this->fieldTypesMap[z * PLANE_WIDTH + x] == FieldType::Point) {
				std::map<std::pair<int, int>, Point*>::iterator it = this->Points.find(std::pair<int, int>(x, z));
				if (it == this->Points.end()) { // no entry at x, z
					this->Points.insert(
						std::pair<std::pair<int, int>, Point*>(
							std::pair<int, int>(x, z),
							new Point(x, z, POINT_RADIUS, this->pShaderPoint))
					);
				}
			}
		}
	}
}

void Field::createPortals() {
	this->pShaderPortal->diffuseTexture(Texture::LoadShared(TEXTURE_DIRECTORY "portal-green.png"));
	this->Portals.push_back(new Portal(28, 15, Orientation::East, this->pShaderPortal));
	this->Portals.push_back(new Portal(1, 15, Orientation::West, this->pShaderPortal));
}

Vector Field::closestPointPos(Vector origin) {
	float closestDistance = 999999;
	Vector target = Vector(0, 0, 0);
	for (auto const& point : this->Points) {
		Vector diff = origin - point.second->transform().translation();
		if (diff.length() < closestDistance) {
			closestDistance = diff.length();
			target = point.second->transform().translation();
		}

	}
	return target;
}

bool Field::removePoint(int x, int z) {
	if (SHOW_POINTS) {
		bool isPoint = this->fieldTypesMap[z * PLANE_WIDTH + x] == FieldType::Point;
		if (!isPoint) {
			return false;
		}
		this->fieldTypesMap[z * PLANE_WIDTH + x] = FieldType::Free;
        this->particlePopEmitter->trigger(this->Points.at(std::pair<int, int>(x, z))->transform().translation(), 10);
		delete this->Points.at(std::pair<int, int>(x, z));
		this->Points.erase(std::pair<int, int>(x, z));
		return false;
	}
}

bool Field::pointsLeft() {
	return !this->Points.empty() || !SHOW_POINTS;
}

void Field::draw(const Camera camera) {
	if (SHOW_DEBUG_PLANE) this->pPlaneDebug->draw(camera);
	if (SHOW_PLANE) this->pPlane->draw(camera);
    this->particlePopEmitter->draw(camera);
	for (ModelList::iterator wall = this->Walls.begin(); wall != this->Walls.end(); ++wall)
	{
		(*wall)->draw(camera);
	}
	for (PortalList::iterator portal = this->Portals.begin(); portal != this->Portals.end(); ++portal)
	{
		(*portal)->draw(camera);
	}
	for (auto const& point : this->Points) {
		point.second->draw(camera);
	}
}

void Field::update(float dtime) {
    this->particlePopEmitter->update(dtime);
	for (auto const& point : this->Points) {
		point.second->update(dtime);
	}
	for (PortalList::iterator portal = this->Portals.begin(); portal != this->Portals.end(); ++portal)
	{
		(*portal)->update(dtime);
	}
}

void Field::end()
{
	delete[] this->fieldTypesMap;
	for (auto const& point : this->Points) {
		delete point.second;
	}
	this->Points.clear();
	for (ModelList::iterator it = Walls.begin(); it != Walls.end(); ++it) {
		delete* it;
	}
	this->Walls.clear();
	for (PortalList::iterator it = Portals.begin(); it != Portals.end(); ++it) {
		delete* it;
	}
	this->Portals.clear();
	delete this->pPlane;
	delete this->pPlaneDebug;
	delete this->pShaderWall;
	delete this->pShaderPortal;
	delete this->pShaderPoint;
}


void Field::initWallPositions() {
	if (SHOW_WALLS) {
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
			{{0, 14}, {1, 3}}, // portal wall left
			{{29, 14}, {1, 3}}, // portal wall right
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
	else {
		this->wallPositions = {};
	}
}

FieldType Field::getFieldType(int x, int z) {
	if (x < 0 || z < 0 || x >= PLANE_WIDTH|| z >= PLANE_DEPTH) {
		return FieldType::OutOfField;
	}
	return this->fieldTypesMap[z * PLANE_WIDTH + x];
}

FieldType Field::getFieldType(Vector position) {
	std::pair<int, int> posOnField = this->getPosOnField(position);
	return this->getFieldType(posOnField.first, posOnField.second);
}

void Field::initFieldTypesMap() {
	this->fieldTypesMap = new FieldType[PLANE_WIDTH * PLANE_DEPTH];
	std::fill_n(this->fieldTypesMap, PLANE_WIDTH * PLANE_DEPTH, FieldType::Free);

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
				this->fieldTypesMap[z * PLANE_WIDTH + x] = FieldType::Wall;
			}
		}
	}

	// set points where no walls - and free where no points
	for (int z = 0; z < PLANE_DEPTH; z++) {
		for (int x = 0; x < PLANE_WIDTH; x++) {
			if (this->fieldTypesMap[z * PLANE_WIDTH + x] != FieldType::Wall) {
				this->fieldTypesMap[z * PLANE_WIDTH + x] = FieldType::Point;
				// no points outside the outer walls
				if (x == 0 || x == 29 || z == 0 || z == 32) {
					this->fieldTypesMap[z * PLANE_WIDTH + x] = FieldType::Free;
				}
				// no points in centrum
				if (x != 7 && x != 22 && z >= 10 && z <= 20) {
					this->fieldTypesMap[z * PLANE_WIDTH + x] = FieldType::Free;
				}
				// no points at this specific position (why Namco?)
				if ((x == 14 || x == 15) && z == 24) {
					this->fieldTypesMap[z * PLANE_WIDTH + x] = FieldType::Free;
				}

				// portals
				if (z == 15 && (x == 1 || x == 28)) {
					this->fieldTypesMap[z * PLANE_WIDTH + x] = FieldType::Portal;
				}
			}
		}
	}
	//this->printFieldTypesMap();
	return;
}

void Field::printFieldTypesMap() {
	for (int z = 0; z < PLANE_DEPTH; z++) {
		std::string leading0 = (z >= 10 ? "" : "0");
		std::cout << leading0 << z << " > ";
		for (int x = 0; x < PLANE_WIDTH; x++) {
			if (this->fieldTypesMap[z * PLANE_WIDTH + x] == FieldType::Wall) {
				std::cout << "x";
			}
			else if (this->fieldTypesMap[z * PLANE_WIDTH + x] == FieldType::Point) {
				std::cout << "*";
			}
			else {
				std::cout << "-";
			}
		}
		std::cout << " <" << std::endl;
	}
}

std::pair<int, int> Field::getPosOnField(Vector position) {
	int x = position.X + ((float)PLANE_WIDTH / 2.0f - 0.5f);
	int z = position.Z + ((float)PLANE_DEPTH / 2.0f - 0.5f);
	return std::pair<int, int>(x, z);
}
