#pragma once  
#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(Location) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Red, true);  
#define DRAW_SPHERE_SINGLE_FRAME(Location) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Red, false, -1.f);
#define DRAW_LINE(StartLocation, EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, true, -1.f, 0, 5.f);
#define DRAW_LINE_SINGLE_FRAME(StartLocation, EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, -1.f, 0, 5.f);
#define DRAW_POINT(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 10.f, FColor::Blue, true); 
#define DRAW_POINT_SINGLE_FRAME(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 10.f, FColor::Blue, false, -1.f);
#define DRAW_VECTOR(StartLocation,EndLocation) if(GetWorld())\
	  { \
		  DrawDebugPoint(GetWorld(), StartLocation, 10.f, FColor::Blue, true); \
		  DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, true, -1.f, 0, 5.f); \
		  DrawDebugPoint(GetWorld(), EndLocation, 10.f, FColor::Red, true); \
	  }
#define DRAW_VECTOR_SINGLE_FRAME(StartLocation,EndLocation) if(GetWorld())\
	  { \
		  DrawDebugPoint(GetWorld(), StartLocation, 10.f, FColor::Blue, false, -1.f); \
		  DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, -1.f, 0, 5.f); \
		  DrawDebugPoint(GetWorld(), EndLocation, 10.f, FColor::Red, false, -1.f); \
	  }