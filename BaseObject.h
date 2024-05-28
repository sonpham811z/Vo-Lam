#pragma once
#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include"commonFuc.h"

class BaseObject
{
public:
	BaseObject();
	~BaseObject();
	void SetRect(const int& x, const int& y)
	{
		rect_.x = x;
		rect_.y = y;
	}

	SDL_Rect GetRect() const
	{
		return rect_;
	}

	SDL_Texture* GetObject() const
	{
		return p_object_;
	}

	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Render(SDL_Renderer* des, int x, int y);
	void Free();

	int GetWidth() const
	{
		return rect_.w;
	}

	int GetHeight() const
	{
		return rect_.h;
	}
protected:
	SDL_Texture* p_object_;
	SDL_Rect rect_;
};
#endif // !BASE_OBJECT_H_
