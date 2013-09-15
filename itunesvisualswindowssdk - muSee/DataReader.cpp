#include "DataReader.h"

//=========================================================================================
//Core function
//=========================================================================================
DataReader :: DataReader(){
	loadingStage = 0;

	Init();

	/*
	animationTextureDQue.clear();
	verticalanimationTextureDQue.clear();
	backgroundTextureDQue.clear();
	flyingAnimationTextureDQue.clear();
	*/
}

void DataReader::Destroy(){
	//free all the vectors
	FreeAll(textureVec);

	animationTextureDQue.clear();
	verticalanimationTextureDQue.clear();
	backgroundTextureDQue.clear();
	flyingAnimationTextureDQue.clear();
	
	textureVec.clear();
	//free string buffer
	free(filename);
}

//=========================================================================================
//Loader function
//=========================================================================================
void DataReader::Init(){
	//try to load the data
	fileStringBuffer.clear();
	theDirectoryBuffer = ExePath();
	filename = (char*) malloc(maxDirectorySize);

	//texture vect
	textureVec.clear();
	//ScriptReader::init(theDirectoryBuffer);
}

//load texture parameters (available only after OpenGL is initialized)
bool DataReader::TextureLoadDrive(){
	
	readingSuccessful = GeneralLoader(0);//animation 0
	readingSuccessful = GeneralLoader(1);//background 0
	readingSuccessful = GeneralLoader(2);//vertical 0
	readingSuccessful = GeneralLoader(3);//vertical 1
	readingSuccessful = GeneralLoader(4);//animation 1
	readingSuccessful = GeneralLoader(5);//animation 2
	return readingSuccessful;
}

//----------------------------------------------
//General Loader
bool DataReader::GeneralLoader(char * thename, TextureType theType, LoadingOption repeatOpt)
{
	bool loadSuccessful = false;
	
	GetPathToData(thename);
	loadSuccessful = LoadRawDecodedData();
	
	if(loadSuccessful){
		TextureImage atexturebuffer;
		memset(&atexturebuffer, 0, sizeof(TextureImage));
		//onlyUsingPrecompiledAnimationVariables = LoadTGA(&atexturebuffer, X_Clamp);
		loadSuccessful = LoadPNG(&atexturebuffer, repeatOpt);
		
		atexturebuffer.texTypeID = theType;
		//categorize index
		switch(theType){
			case AnimationTexture:
				animationTextureDQue.push_back(textureVec.size());
			break;
			case BackgroundTexture:
				backgroundTextureDQue.push_back(textureVec.size());
			break;
			case VerticalAnimationTexture:
				verticalanimationTextureDQue.push_back(textureVec.size());
			break;
			case FlyingAnimationTexture:
				flyingAnimationTextureDQue.push_back(textureVec.size());
			break;
			default:
				animationTextureDQue.push_back(textureVec.size());
				atexturebuffer.texTypeID = AnimationTexture;
		}
		//attributes
		atexturebuffer.texName = thename;
		atexturebuffer.texRepeatOpt = repeatOpt;

		//load precompiled data if defined
		FillTexInfoDrive(&atexturebuffer);

		textureVec.push_back(atexturebuffer); //copy to vector storage
		loadedTexutreID.push_back(atexturebuffer.texID);
	
	}

	return loadSuccessful;
}

bool DataReader::GeneralLoader(int PreCompiledind){
	char *  thename;
	TextureType theType;
	LoadingOption repeatOpt;

	switch(PreCompiledind){
	case 0:
		thename = "leaf_draw.png";
		theType = AnimationTexture;
		repeatOpt = X_Clamp;
		GetPathToData(thename);
		break;
	case 1:
		thename = "CCs_background1.png";
		theType = BackgroundTexture;
		repeatOpt = X_Repeat;
		GetPathToData(thename);
		break;
	case 2:
		thename = "grass1.png";
		theType = VerticalAnimationTexture;
		repeatOpt = X_Clamp;
		GetPathToData(thename);
		break;
	case 3:
		thename = "square.png";
		theType = VerticalAnimationTexture;
		repeatOpt = X_Clamp;
		GetPathToData(thename);
		break;
	case 4:
		thename = "flower1.png";
		theType = AnimationTexture;
		repeatOpt = X_Clamp;
		GetPathToData(thename);
		break;
	case 5:
		thename = "flowerMode1.png";
		theType = AnimationTexture;
		repeatOpt = X_Clamp;
		GetPathToData(thename);
		break;
	}

	bool loadSuccessful = false;
	
	loadSuccessful = LoadRawDecodedData();
	
	if(loadSuccessful){
		TextureImage atexturebuffer;
		memset(&atexturebuffer, 0, sizeof(TextureImage));
		//onlyUsingPrecompiledAnimationVariables = LoadTGA(&atexturebuffer, X_Clamp);
		loadSuccessful = LoadPNG(&atexturebuffer, repeatOpt);
		
		atexturebuffer.texTypeID = theType;
		//categorize index
		switch(theType){
			case AnimationTexture:
				animationTextureDQue.push_back(textureVec.size());
			break;
			case BackgroundTexture:
				backgroundTextureDQue.push_back(textureVec.size());
			break;
			case VerticalAnimationTexture:
				verticalanimationTextureDQue.push_back(textureVec.size());
			break;
			case FlyingAnimationTexture:
				flyingAnimationTextureDQue.push_back(textureVec.size());
			break;
			default:
				animationTextureDQue.push_back(textureVec.size());
				atexturebuffer.texTypeID = AnimationTexture;
		}
		//attributes
		atexturebuffer.texName = thename;
		atexturebuffer.texRepeatOpt = repeatOpt;

		//load precompiled data if defined
		FillTexInfoDrive(&atexturebuffer);
		FillTexInfo(&atexturebuffer.texParameter, PreCompiledind);

		textureVec.push_back(atexturebuffer); //copy to vector storage
		loadedTexutreID.push_back(atexturebuffer.texID);
	
	}

	return loadSuccessful;



}

//precompiled info filler driver
void DataReader::FillTexInfoDrive(TextureImage * texInfo){
	//if..else if string matching chain

	if(texInfo->texName.compare("leaf_draw.png")==0){
		FillTexInfo(&texInfo->texParameter, 0);
	}
	/*else if(strcmp(thename, "FlyingSpark.png")){
		FillTexInfo(&texInfo->texParameter, 1);
	}*/
	else if(texInfo->texName.compare("CCs_background1.png")==0){
		FillTexInfo(&texInfo->texParameter, 1);
	}else if(texInfo->texName.compare("grass1.png")==0){
		FillTexInfo(&texInfo->texParameter, 2);
	}
}

//filling methods
void DataReader::FillTexInfo(TexParameter * texInfo, int index){
	switch(index){
	case 0: //"leaf_draw.png"
		texInfo->max_frame = abstractLeefMaxFrame;
		texInfo->animationColumns = abstractLeefAnimationColumns;
		texInfo->usingTexture = true;
		texInfo->initialStationary= false;
		texInfo->collidable = false;

		texInfo->xBound = 0;
		texInfo->yBound = 0;

		texInfo->entranceEffectID = Diagonal;

		texInfo->minLength = 0.12;
		texInfo->maxLength = 0.18;

		texInfo->minTimeInS = 0.5;
		texInfo->maxTimeInS = 2.0;
		texInfo->max_AngleChangeInRad = 0.25 * pi;

		//program effect time here
		texInfo->minEntranceTime = 0.3;
		texInfo->maxEntranceTime = 1.2;
		break;

	case 1: 
	case 2: 
	default:
		texInfo->max_frame = 1;
		texInfo->animationColumns = 1;
		texInfo->usingTexture = true;
		texInfo->collidable = false;
		texInfo->initialStationary= true;
		
		texInfo->xBound = 0;
		texInfo->yBound = 0;

		texInfo->entranceEffectID = fading;

		texInfo->minLength = 0.01;
		texInfo->maxLength = 0.01;

		texInfo->minTimeInS = 10;
		texInfo->maxTimeInS = 20000;

		texInfo->minEntranceTime = 0;
		texInfo->maxEntranceTime = 0;

		texInfo->max_AngleChangeInRad = 2*pi;

	};
}


//----------------------------------------------
//General Getter
//return the pointer to the texture inforamtaion container by index and texture type
TextureImage * DataReader::GetTextureInfo(int index, TextureType texType){
	int absolutePostion;
	switch(texType){
			case AnimationTexture:
				absolutePostion = animationTextureDQue[index];
			break;
			case BackgroundTexture:
				absolutePostion = backgroundTextureDQue[index];
			break;
			case VerticalAnimationTexture:
				absolutePostion = verticalanimationTextureDQue[index];
			break;
			case FlyingAnimationTexture:
				absolutePostion = flyingAnimationTextureDQue[index];
			break;
		}

	return SimpleGetTextureInfo(absolutePostion);
}

//return the pointer to the texture inforamtaion container
TextureImage * DataReader::SimpleGetTextureInfo(int absoluteIndex){
	return &textureVec[absoluteIndex];
}

//first level decoding
bool DataReader::LoadRawDecodedData(){
	//first perform cipher decoding
	errno_t err = fopen_s(&file, filename, "rb");                 // Open The TGA File
	if(file==NULL){
		//fclose (file); 
		return false;
	}else{
		bool decoded = decodeFile(file); //calls cipher
		fclose (file); 
		if(decoded)
			return true;
	}
	return false;
}

//------------------------------------------------------------------
//Loader functions (From NeHe Production Tutorial: http://nehe.gamedev.net/tutorial/picking_alpha_blending_alpha_testing_sorting/16005/)
//8 bits Alpha channel is included if the TGA file is 32 bits (24 bits color, 8 bits alpha)
//------------------------------------------------------------------
bool DataReader::LoadTGA(TextureImage *texture, int Option)             // Loads A TGA File Into Memory
{   
    GLubyte     TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};        // Uncompressed TGA Header
    GLubyte     TGAcompare[12];                     // Used To Compare TGA Header
    GLubyte     header[6];                      // First 6 Useful Bytes From The Header
    GLuint      bytesPerPixel;                      // Holds Number Of Bytes Per Pixel Used In The TGA File
    GLuint      imageSize;                      // Used To Store The Image Size When Setting Aside Ram
    GLuint      temp;                           // Temporary Variable
    GLuint      type=GL_RGBA;                       // Set The Default GL Mode To RBGA (32 BPP)
	GLubyte		*imageData;							//Image Data (Up To 32 Bits)

	errno_t err = fopen_s(&file, filename, "rb");  
    if( file==NULL ||                           // Does File Even Exist?
        fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||  // Are There 12 Bytes To Read?
        memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0 ||        // Does The Header Match What We Want?
        fread(header,1,sizeof(header),file)!=sizeof(header))        // If So Read Next 6 Header Bytes
    {
        if (file == NULL)                       // Does The File Even Exist? *Added Jim Strong*
            return FALSE;                       // Return False
        else                                // Otherwise
        {
            fclose(file);                       // If Anything Failed, Close The File
            return FALSE;                       // Return False
        }
    }
 
    texture->width  = header[1] * 256 + header[0];               // Determine The TGA Width  (highbyte*256+lowbyte)
    texture->height = header[3] * 256 + header[2];               // Determine The TGA Height (highbyte*256+lowbyte)
     
    if( texture->width   <=0 ||                       // Is The Width Less Than Or Equal To Zero
        texture->height  <=0 ||                       // Is The Height Less Than Or Equal To Zero
        (header[4]!=24 && header[4]!=32))               // Is The TGA 24 or 32 Bit?
    {
        fclose(file);                           // If Anything Failed, Close The File
        return FALSE;                           // Return False
    }
 
    texture->bpp = header[4];                        // Grab The TGA's Bits Per Pixel (24 or 32)
    bytesPerPixel   = texture->bpp/8;                    // Divide By 8 To Get The Bytes Per Pixel
    imageSize       = texture->width*texture->height*bytesPerPixel;   // Calculate The Memory Required For The TGA Data
 
    imageData=(GLubyte *)malloc(imageSize);         // Reserve Memory To Hold The TGA Data
 
    if( imageData==NULL ||                  // Does The Storage Memory Exist?
        fread(imageData, 1, imageSize, file)!=imageSize)    // Does The Image Size Match The Memory Reserved?
    {
        if(imageData!=NULL)                 // Was Image Data Loaded
            free(imageData);                // If So, Release The Image Data
 
        fclose(file);                           // Close The File
        return FALSE;                           // Return False
    }
 
    for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)          // Loop Through The Image Data
    {                                   // Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
        temp=imageData[i];                  // Temporarily Store The Value At Image Data 'i'
        imageData[i] = imageData[i + 2];        // Set The 1st Byte To The Value Of The 3rd Byte
        imageData[i + 2] = temp;                // Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
    }
 
    fclose (file);                              // Close The File
 
    // Build A Texture From The Data
    glGenTextures(1, &texture[0].texID);                    // Generate OpenGL texture IDs
	loadedTexutreID.push_back(texture->texID);

    glBindTexture(GL_TEXTURE_2D, texture[0].texID);             // Bind Our Texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   // Linear Filtered
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // Linear Filtered
	
	if(Option == X_Clamp)//x direction
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
	else if(Option == X_Repeat)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); //default
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); //y direction
     
    if (texture[0].bpp==24)                         // Was The TGA 24 Bits
    {
        type=GL_RGB;                            // If So Set The 'type' To GL_RGB
    }
 
    glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, imageData);
 
	free(imageData);		// an optimization to free the pointer, no need to keep the imageData

    return true;                                // Texture Building Went Ok, Return True
}

bool DataReader::LoadPNG(TextureImage *texture, int Option){
	vector<unsigned char> buffer, image;
	
	lodepng::load_file(buffer, filename); //load the image file with given filename
	
	unsigned width, height;

	unsigned error = lodepng::decode(image, width, height, buffer);

	if(error != 0){
		return false;
	}
	
	texture->width = width;
	texture->height = height;
	texture->bpp = 32; //for GL_RGBA type
	
	// Texture size must be power of two for the primitive OpenGL version this is written for. Find next power of two.
	size_t u2 = 1; while(u2 < width) u2 *= 2;
	size_t v2 = 1; while(v2 < height) v2 *= 2;
	// Ratio for power of two version compared to actual version, to render the non power of two image with proper size.
	float u3 = (float)width / u2;
	float v3 = (float)height / v2;

	// Make power of two version of the image.
	std::vector<unsigned char> image2(u2 * v2 * 4);
	for(size_t y = 0; y < height; y++)
	for(size_t x = 0; x < width; x++)
	for(size_t c = 0; c < 4; c++)
	{
		image2[4 * u2 * (height - y - 1) + 4 * x + c] = image[4 * width * y + 4 * x + c];
	}

	// Build A Texture From The Data
	
	glGenTextures(1, &texture->texID);                    // Generate OpenGL texture IDs
	loadedTexutreID.push_back(texture->texID);
	
    glBindTexture(GL_TEXTURE_2D, texture->texID);             // Bind Our Texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   // Linear Filtered
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // Linear Filtered
	
	
	switch(Option){
		case X_Repeat:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		break;
		case X_Clamp:
		default:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); //default;
	};
	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); //y direction
	glTexImage2D(GL_TEXTURE_2D, 0, 4, u2, v2, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image2[0]);

	FreeAll( buffer );
	FreeAll( image );
	FreeAll( image2 );
    return true;                                // Texture Building Went Ok, Return True
}

void DataReader::RemoveAllTextures(){
	while(loadedTexutreID.size()>0){
		glDeleteTextures(1, &(GLuint)loadedTexutreID.back());
		loadedTexutreID.pop_back();
	}

	animationTextureDQue.clear();
	verticalanimationTextureDQue.clear();
	backgroundTextureDQue.clear();
	flyingAnimationTextureDQue.clear();

	FreeAll(textureVec);
}

//=========================================================================================
//Helper function
//=========================================================================================
//Get the max frame
int DataReader::GetMaxFrame(int Index){
	if(!readingSuccessful){

		return defaultMaxFrame;
	}
	return abstractLeefMaxFrame;
}

int DataReader::InfoOutMessage(){
	return loadingStage;
}

void DataReader::GetPathToData(char* thename){
	fileStringBuffer = theDirectoryBuffer; // reinitialize
	fileStringBuffer += intermediateFilePath;
	fileStringBuffer += thename;
	filename = strcpy( filename, fileStringBuffer.c_str());
}
