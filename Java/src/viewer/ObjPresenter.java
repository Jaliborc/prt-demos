package viewer;
import static org.lwjgl.opengl.GL11.*;
import mintools.parameters.DoubleParameter;
import formats.obj.*;
import graphics.Presenter;
import math.Vector;

class ObjPresenter extends Presenter {
	public ObjPresenter(String file) throws Exception {
		model = new Model(file);
		alpha =  new DoubleParameter("Alpha", .6, 0, 1);
		normalScale = new DoubleParameter("Normal Scale", .05, .001, 2);
		
		add(alpha.getSliderControls(false));
		add(normalScale.getSliderControls(false));
	}
	
	public void draw() {
		drawPoints();
		drawNormals();
		drawWireframe();
		drawSurface();
	}
	
	void drawPoints() {
		glPointSize(4f);
		glColor3d(0, 1, 0);
		glBegin(GL_POINTS);
		drawVertices();
		glEnd();
	}
	
	void drawNormals() {
		glColor3d(1, 0, 0);
		glBegin(GL_LINES);
		
		for (Face face : model.faces)
			for (Vertex vertex : face) {
				Vector normal = new Vector(vertex.normal);
				normal.scale(normalScale.getFloatValue());
				normal.add(vertex.position);
				
				glVertex3f(vertex.position.x, vertex.position.y, vertex.position.z);
				glVertex3f(normal.x, normal.y, normal.z);
			}
		
		glEnd();
	}
	
	void drawWireframe() {
		glColor3d(0, 0, 1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_TRIANGLES);
		drawVertices();	
		glEnd();
	}
	
	void drawSurface() {
		glEnable(GL_BLEND);
		glColor4d(.6, .6, .6, alpha.getValue());
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_TRIANGLES);
		drawVertices();
		glEnd();
		glDisable(GL_BLEND);
	}
	
	void drawVertices() {
		for (Face face : model.faces)
			for (Vertex vertex : face)
				glVertex3f(vertex.position.x, vertex.position.y, vertex.position.z);
	}
	
	Model model;
	DoubleParameter alpha, normalScale;
}
