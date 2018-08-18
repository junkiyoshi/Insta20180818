#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(39);
	ofSetColor(239);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	this->font_size = 120;
	this->font.loadFont("fonts/Kazesawa-bold.ttf", this->font_size, true, true, true);

	this->charactors = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (int x = 72; x < ofGetWidth(); x += 144) {

		for (int y = 72; y < ofGetHeight(); y += 144) {

			float noise_value = ofMap(ofNoise(ofRandom(100), ofGetFrameNum() * 0.002), 0, 1, 0, 1000);
			int index = noise_value * 0.01;
			int next_index = (index + 1) % this->charactors.size();
			int progress = noise_value - index * 100;

			this->draw_blend_charactor(ofPoint(x, y), this->charactors[index], this->charactors[next_index], progress);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw_blend_charactor(ofPoint point, char c1, char c2, float progress) {

	int total_sample_count = 60;

	ofPushMatrix();
	ofTranslate(point);

	ofPath path = this->font.getCharacterAsPoints(c1, true, false);
	vector<ofPolyline> outline = path.getOutline();
	int sample_count = total_sample_count / outline.size();
	vector<ofPoint> points;
	for (int outline_index = 0; outline_index < (int)outline.size(); outline_index++) {

		outline[outline_index] = outline[outline_index].getResampledByCount(sample_count);
		vector<glm::vec3> vertices = outline[outline_index].getVertices();
		for (size_t vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

			ofPoint point(vertices[vertices_index].x - this->font_size * 0.5, vertices[vertices_index].y + this->font_size * 0.5, vertices[vertices_index].z);
			points.push_back(point);
		}
	}

	ofPath next_path = this->font.getCharacterAsPoints(c2, true, false);
	vector<ofPolyline> next_outline = next_path.getOutline();
	int next_sample_count = total_sample_count / next_outline.size();
	vector<ofPoint> next_points;
	for (int outline_index = 0; outline_index < (int)next_outline.size(); outline_index++) {

		next_outline[outline_index] = next_outline[outline_index].getResampledByCount(next_sample_count);
		vector<glm::vec3> vertices = next_outline[outline_index].getVertices();
		for (size_t vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

			ofPoint point(vertices[vertices_index].x - this->font_size * 0.5, vertices[vertices_index].y + this->font_size * 0.5, vertices[vertices_index].z);
			next_points.push_back(point);
		}
	}

	vector<ofPoint> draw_points;
	for (int i = 0; i < total_sample_count; i++) {

		if (i < points.size() && i < next_points.size()) {

			ofPoint p = (next_points[i] - points[i]) * progress * 0.01 + points[i];
			draw_points.push_back(p);
		}
	}

	for (int i = 0; i < draw_points.size(); i++) {

		ofDrawCircle(draw_points[i], 2);
		for (int j = i + 1; j < draw_points.size(); j++) {

			if (draw_points[i].distance(draw_points[j]) < 15) {

				ofDrawLine(draw_points[i], draw_points[j]);
			}
		}
	}

	ofPopMatrix();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}