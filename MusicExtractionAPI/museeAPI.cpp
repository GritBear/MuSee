#include "museeAPI.h"
#include <stdexcept>

namespace musee
{
	void museeAPI::init(MelodyExtractionPram * param){
		melody = new MelodyExtraction(param);
		rhythmic = new RhythmicExtract(param);
	}

	void museeAPI::update(RenderVisualData * RenderData){
		melody->Update(RenderData);
		rhythmic->Update(RenderData);
	};
}