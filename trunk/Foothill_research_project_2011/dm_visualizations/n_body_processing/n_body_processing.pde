///////////////////////////////////////////////////////////////////////////////
// GUI stuff
///////////////////////////////////////////////////////////////////////////////
import controlP5.*;

ControlP5 controlP5;

DropdownList p1, p2;
///////////////////////////////////////////////////////////////////////////////
import javax.swing.JFileChooser;
///////////////////////////////////////////////////////////////////////////////
import processing.opengl.*;
///////////////////////////////////////////////////////////////////////////////
// PeasyCam - a good camera.
import peasy.*;
///////////////////////////////////////////////////////////////////////////////

int screen_width = 800;
int screen_height = 800;

float xcenter = screen_width/2.;
float ycenter = screen_height/2.;
float zcenter = 0.0;

int width = 600;
int height = 600;
int depth = 500;

float xnorm = 1.0;
float ynorm = 1.0;
float znorm = 1.0;

String[] lines;
float nlines;
int nparticles;
int nlines_in_time_slice;
int[] radii = new int[1000];
float[] pos_ranges = new float[6];

String[] filenames = new String[0];
String infile;

int dt = 0;

boolean process_file = false;

int process_every_nslices = 1;

BufferedReader reader;
//BufferedReader reader = createReader("/home/bellis/sketchbook/display_dark_matter_animation_csv_files_input/data/output.csv");

JFileChooser chooser = new JFileChooser();

PeasyCam cam;
PMatrix3D currCameraMatrix;
PGraphics3D g3; 

///////////////////////////////////////////////////////////////////////////////
// Grabbed this from http://wiki.processing.org/w/BufferedReader
///////////////////////////////////////////////////////////////////////////////
public BufferedReader createReader(String filename) {
    try {
        InputStream is = openStream(filename);
        if (is == null) {
            System.err.println(filename + " does not exist or could not be read");
            return null;
        }
        return createReader(is);
    } catch (Exception e) {
        if (filename == null) {
            System.err.println("Filename passed to reader() was null");
        } else {
            System.err.println("Couldn't create a reader for " + filename);
        }
    }
    return null;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int cnt = 0;

void setup() 
{

    // For better controlP5 fonts
    hint(ENABLE_NATIVE_FONTS);

    ///////////////////////////////////////////////////////////////////////////
    // Read in the config file to set the ranges on the scale of the positions
    // vectors.
    ///////////////////////////////////////////////////////////////////////////
    lines = loadStrings("visual_config.dat");
    nlines = lines.length;
    for (int i=0;i<nlines;i++)
    {
        String[] vals = split(lines[i], ',');
        if ( vals[0].charAt(0) == '#' ) // Allow for comments
        {
            continue;
        }
        // Only need 6 values for (x,y,z) lo and hi range
        else if ( vals.length==6 ) 
        {
            for (int j=0;j<6;j++)
            {
                pos_ranges[j] = float(vals[j]);
            }
        }
        else
        {
            println("Visualization config file not formatted properly!");
            println("\n\texiting\n");
            exit();
        }
    }

    // Set the normalization terms
    xnorm = float(width)/(abs(pos_ranges[0]-pos_ranges[1]));
    ynorm = float(height)/(abs(pos_ranges[2]-pos_ranges[3]));
    znorm = float(depth)/(abs(pos_ranges[4]-pos_ranges[5]));
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    //size(screen_width,screen_height,P3D);
    size(screen_width,screen_height,OPENGL);
    g3 = (PGraphics3D)g;
    cam = new PeasyCam(this, xcenter, ycenter, -depth, 2.0*depth);
    //cam = new PeasyCam(this, 100);
    cam.setMinimumDistance(50);
    cam.setMaximumDistance(4000);

    //size(screen_width,screen_height,OPENGL);
    frameRate(30);
    //noLoop();
    //smooth();
    background(0);

    //lines = loadStrings("output.csv");
    //nlines = lines.length;

    // Path
    String path = dataPath("");
    println("Listing all filenames in a directory: ");
    println(path);
    String[] temp_filenames = listFileNames(path);
    int nfiles = temp_filenames.length;
    println(temp_filenames);
    int j = 0;
    for (int i=0;i<nfiles;i++)
    {
        if (temp_filenames[i].endsWith("csv"))
        {
            println(temp_filenames[i]);
            filenames = append(filenames, temp_filenames[i]);
            j++;
        }
    }
    println(filenames);

    controlP5 = new ControlP5(this);
    p1 = controlP5.addDropdownList("myList-p1",50,50,120,120);
    customize(p1);
    p2 = controlP5.addDropdownList("myList-p2",220,50,120,120);
    customize_filelist(p2);

    controlP5.setAutoDraw(false);

}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void draw() {

    int mCount = 0;

    background(0);
    lights();

    if (process_file)
    {
        draw_time_slice();
    }

    dt++;

    //controlP5.draw();
    // makes the gui stay on top of elements
    // drawn before.
    hint(DISABLE_DEPTH_TEST);
    gui();

}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void gui() {
    currCameraMatrix = new PMatrix3D(g3.camera);
    camera();
    controlP5.draw();
    g3.camera = currCameraMatrix;
    }
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void draw_time_slice()
{

    // Color of particles 
    //fill(182,62,29); // red
    fill(255,255,0); // red

    // Make the particles look better.
    noStroke();

    int i;

    String line = "test,test";

    for (int k=0;k<process_every_nslices;k++)
    {
        line = "DEFAULTLINE";
        try{
            line = reader.readLine();
            //println(line);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
        //String[] vals = split(lines[dt], ',');
    }


    if (line=="DEFAULTLINE")
    {
        println("Reached end of file 0.");
        reader = createReader(infile);
        dt = -1;
        //exit();
    }
    else
    {

        String[] vals = split(line, ',');

        if (vals==null || vals.length==0)
        {
            println("Reached end of file 1.");
            reader = createReader(infile);
            dt = -1;
            //exit();
        }
        else
        {

            for (i=0;i<nparticles;i++)
            {

                //println(dt+" "+vals[0]);
                if (dt!=0 && vals.length>0)
                {

                    //println("here: " + vals[0]);
                    float radius = float(radii[i]);
                    float x = xcenter + float(vals[1+i*3])*xnorm;
                    float y = ycenter - float(vals[2+i*3])*ynorm;
                    float z = zcenter - float(vals[3+i*3])*znorm;
                    //float z = float(vals[6+i*5])*depth + 100.0;

                    //println("xyz: " + x + " " + y + " " + z);

                    float zcol = 255*(z + depth)/(2.0*depth);
                    int rcol = int(zcol);
                    int gcol = int(255* sin(3.14159*zcol/255.0));
                    int bcol = int(255-zcol);
                    //println(zcol);
                    fill(rcol,gcol,bcol); 

                    pushMatrix();
                    translate(x, y, -z);
                    sphere(radius);
                    popMatrix();

                }
            }
        }

    }



    //controlP5.draw();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void customize(DropdownList ddl) {
    //ddl.setBackgroundColor(color(190));
    ddl.setItemHeight(15);
    ddl.setBarHeight(15);
    //ddl.setHeight(300);
    //ddl.setWidth(100);
    ddl.captionLabel().set("Speed");
    ddl.captionLabel().style().marginTop = 3;
    //ddl.captionLabel().style().marginLeft = 6;
    ddl.valueLabel().style().marginTop = 3;
    //ddl.valueLabel().style().marginLeft = 6;
    // Make some drop down items
    String[] names = {"slow","medium","fast"};
    for(int i=0;i<3;i++) {
        //ddl.addItem("item "+i,i);
        ddl.addItem(names[i],i);
    }
    ddl.setColorBackground(color(255,128));
    //ddl.setColorForeground(color(255));
    //ddl.setColorLabel(color(0));
    ddl.setColorActive(color(0,0,255,128));
}

///////////////////////////////////////////////////////////////////////////////
void customize_filelist(DropdownList ddl) {
    //ddl.setBackgroundColor(color(190));
    ddl.setItemHeight(15);
    ddl.setBarHeight(15);
    //ddl.setHeight(300);
    ddl.setWidth(200);
    ddl.captionLabel().set("Choose a file");
    ddl.captionLabel().style().marginTop = 3;
    //ddl.captionLabel().style().marginLeft = 6;
    ddl.valueLabel().style().marginTop = 3;
    //ddl.valueLabel().style().marginLeft = 6;
    // Make some drop down items
    //String[] names = {"slow","medium","fast"};
    int nfiles = filenames.length;
    for(int i=0;i<nfiles;i++) {
        //ddl.addItem("item "+i,i);
        ddl.addItem(filenames[i],i);
    }
    ddl.setColorBackground(color(255,128));
    //ddl.setColorForeground(color(255));
    //ddl.setColorLabel(color(0));
    ddl.setColorActive(color(0,0,255,128));
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void controlEvent(ControlEvent theEvent) {
    // PulldownMenu is if type ControlGroup.
    // A controlEvent will be triggered from within the ControlGroup.
    // therefore you need to check the originator of the Event with
    // if (theEvent.isGroup())
    // to avoid an error message from controlP5.

    if (theEvent.isGroup()) {
        // check if the Event was triggered from a ControlGroup
        println(theEvent.group().value()+" from "+theEvent.group());
        println("CLICKED");
        println(theEvent.group());
        println(theEvent.name());
    } else if(theEvent.isController()) {
        println(theEvent.controller().value()+" from "+theEvent.controller());
    }

    if (theEvent.name() == "myList-p1")
    {
        if (theEvent.group().value()==0)
        {
            process_every_nslices = 1;
        }
        else if (theEvent.group().value()==1)
        {
            process_every_nslices = 100;
        }
        else if (theEvent.group().value()==2)
        {
            process_every_nslices = 500;
        }
    }
    if (theEvent.name() == "myList-p2")
    {
        int index = int(theEvent.group().value());
        infile = filenames[index];
        reader = createReader(infile);
        process_file = true;
        dt = 0;

        //String[] vals = split(lines[0], ' ');
        String line = "DEFAULTLINE";
        try{
            line = reader.readLine();
            //println(line);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }

        nparticles = int(line);
        println("nparticles: " + nparticles);
        dt++;

        nlines_in_time_slice = 1;
        for (int n=0;n<nparticles;n++)
        {
            radii[n] = 16;
        }

    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// This function returns all the files in a directory as an array of Strings  
///////////////////////////////////////////////////////////////////////////////
String[] listFileNames(String dir) {
    File file = new File(dir);
    if (file.isDirectory()) {
        String names[] = file.list();
        return names;
    } else {
        // If it's not a directory
        return null;
    }
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// This function returns all the files in a directory as an array of File objects
// This is useful if you want more info about the file
///////////////////////////////////////////////////////////////////////////////
File[] listFiles(String dir) {
    File file = new File(dir);
    if (file.isDirectory()) {
        File[] files = file.listFiles();
        return files;
    } else {
        // If it's not a directory
        return null;
    }
}

