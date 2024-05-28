#include "commonFuc.h"
#include "BaseObject.h"
#include "Player.h"
#include "Quaivat.h"
#include "NguHanh.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <thread>
#include <chrono>
using namespace std;

BaseObject g_background;
BaseObject g_image_button_1;
BaseObject g_image_button_2;
BaseObject g_image_button_3;
BaseObject g_image_button_4;
BaseObject g_background_2;
BaseObject g_background_3;
BaseObject b_return;
BaseObject win;
BaseObject lose;

BaseObject conlon;
BaseObject thieulam;
BaseObject vodang;
BaseObject ngamy;
BaseObject ngudoc;
BaseObject caibang;
BaseObject duongmon;
BaseObject thiennhan;
BaseObject thuyyen;
BaseObject minhgiao;
BaseObject daulinh;
BaseObject quaithuong;

bool use_bg2 = false;
bool use_bg3 = false;
bool use_bg4 = false;
bool use_bg1 = true;
bool show_single_image = false;
bool combat_mode = false;

BaseObject* clicked_image = nullptr;
std::string g_inputText = ""; // Chuỗi nhập liệu;

std::string bang = "";
std::string type = "";
std::string he = "";
std::string input_text1 = "";
std::string input_text2 = "";
std::string input_text3 = "";
std::string input_text4 = "";
std::string input_text5 = "";
std::string input_text6 = "";
std::string input_text7 = "";

bool is_text_box1_selected = false;
bool is_text_box2_selected = false;
bool is_text_box3_selected = false;
bool is_text_box4_selected = false;
bool is_text_box5_selected = false;
bool is_text_box6_selected = false;
bool is_text_box7_selected = false;

std::vector<std::string> names;
std::vector<std::string> names_quai;
std::vector<std::string> bangs;
std::vector<int> levels;
std::vector<int> levels_quai;
std::vector<std::string> hes;
std::vector<std::string> hes_quai;
std::vector<std::string> types;
std::vector<int> vitri;
vector<NhanVat*> characters;

double calculateDamage(const NhanVat& attacker, const NhanVat& defender) {
    double damage = attacker.getDamage();
    if (NguHanh::isSinh(attacker.getElement(), defender.getElement())) {
        damage *= 1.1;
    }
    else if (NguHanh::isKhac(attacker.getElement(), defender.getElement())) {
        damage *= 1.2;
    }
    else if (NguHanh::isKhac(defender.getElement(), attacker.getElement())) {
        damage *= 0.8;
    }
    return damage;
}

bool compareNhanVat(NhanVat* a, NhanVat* b) {
    return a->getDamage() > b->getDamage();
}

bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);

    if (ret < 0)
    {
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("Vo lam truyen ki", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGTH, SDL_WINDOW_SHOWN);

    if (g_window == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL)
        {
            success = false;
        }
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags))
            {
                success = false;
            }

            if (TTF_Init() == -1)
            {
                std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
                success = false;
            }
            else
            {
                g_font_18 = TTF_OpenFont("font/roboto.ttf", 18);
                if (g_font_18 == NULL)
                {
                    std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
                    success = false;
                }
                g_font_13 = TTF_OpenFont("font/roboto.ttf", 14);
                if (g_font_13 == NULL)
                {
                    std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
                    success = false;
                }
            }
        }
    }

    return success;
}

bool LoadBackGround(std::string path, BaseObject& x)
{
    return x.LoadImg(path, g_screen);
}

bool LoadImage(std::string path, BaseObject& x)
{
    return x.LoadImg(path, g_screen);
}

void ShowIntro(SDL_Renderer* renderer, const char* imagePath, int displayTime) {
    BaseObject introImage;
    if (!introImage.LoadImg(imagePath, renderer)) {
        std::cerr << "Failed to load intro image!" << std::endl;
        return;
    }

    SDL_RenderClear(renderer);
    introImage.Render(renderer, 0, 0);
    SDL_RenderPresent(renderer);

    std::this_thread::sleep_for(std::chrono::seconds(displayTime));
}

void RenderText(std::string text, int x, int y, TTF_Font* font)
{
    SDL_Color textColor = { 0, 0, 0 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (textSurface != NULL)
    {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(g_screen, textSurface);
        if (textTexture != NULL)
        {
            SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };
            SDL_RenderCopy(g_screen, textTexture, NULL, &renderQuad);
            SDL_DestroyTexture(textTexture);
        }
        SDL_FreeSurface(textSurface);
    }
}

void close()
{
    g_background.Free();
    g_image_button_1.Free();
    g_image_button_2.Free();
    g_image_button_3.Free();
    g_image_button_4.Free();

    TTF_CloseFont(g_font_18);
    g_font_18 = NULL;
    TTF_CloseFont(g_font_13);
    g_font_13 = NULL;
    TTF_Quit();

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
}

void HandleTextInput(SDL_Event& e)
{
    if (e.type == SDL_TEXTINPUT)
    {
        g_inputText += e.text.text;
    }
    else if (e.type == SDL_KEYDOWN)
    {
        if (e.key.keysym.sym == SDLK_BACKSPACE && g_inputText.length() > 0)
        {
            g_inputText.pop_back();
        }
    }
}

#undef main
int main(int argc, char* argv[])
{
    if (!InitData()) {
        std::cerr << "Failed to initialize!" << std::endl;
        return -1;
    }

    ShowIntro(g_screen, "image/intro.png", 4);
    SDL_DestroyWindow(g_window);

    if (!InitData())
    {
        std::cerr << "Failed to initialize!" << std::endl;
        return -1;
    }

    if (!LoadBackGround("image/volam.jpg", g_background))
    {
        std::cerr << "Failed to load background!" << std::endl;
        return -1;
    }

    if (!LoadImage("image/b1.png", g_image_button_1))
    {
        std::cerr << "Failed to load image 1!" << std::endl;
        return -1;
    }

    if (!LoadImage("image/b2.png", g_image_button_2))
    {
        std::cerr << "Failed to load image 2!" << std::endl;
        return -1;
    }

    if (!LoadImage("image/b3.png", g_image_button_3))
    {
        std::cerr << "Failed to load image 3!" << std::endl;
        return -1;
    }

    if (!LoadImage("image/b4.png", g_image_button_4))
    {
        std::cerr << "Failed to load image 4!" << std::endl;
        return -1;
    }

    if (!LoadBackGround("image/volam1.jpg", g_background_2))
    {
        std::cerr << "Failed to load background 2!" << std::endl;
        return -1;
    }

    if (!LoadImage("image/b_return.png", b_return))
    {
        std::cerr << "Failed to load return button!" << std::endl;
        return -1;
    }

    if (!LoadImage("image/conlon.png", conlon))
    {
        std::cerr << "Failed to load conlon image!" << std::endl;
        return -1;
    }

    if (!LoadImage("image/thieulam.png", thieulam))
    {
        std::cerr << "Failed to load thieulam image!" << std::endl;
        return -1;
    }

    if (!LoadImage("image/vodang.png", vodang))
    {
        std::cerr << "Failed to load vodang image!" << std::endl;
        return -1;
    }

    if (!LoadImage("image/ngamy.jpg", ngamy))
    {
        std::cerr << "Failed to load ngamy image!" << std::endl;
        return -1;
    }

    if (!LoadImage("image/ngudoc.jpg", ngudoc))
    {
        std::cerr << "Failed to load ngudoc image!" << std::endl;
        return -1;
    }

    if (!LoadImage("image/caibang.jpg", caibang))
    {
        std::cerr << "Failed to load caibang image!" << std::endl;
        return -1;
    }

    if (!LoadImage("image/duongmon.jpg", duongmon))
    {
        std::cerr << "Failed to load duongmon image!" << std::endl;
        return -1;
    }

    if (!LoadImage("image/thiennhan.jpg", thiennhan))
    {
        std::cerr << "Failed to load thiennhan image!" << std::endl;
        return -1;
    }

    if (!LoadImage("image/thuyyen.jpg", thuyyen))
    {
        std::cerr << "Failed to load thuyyen image!" << std::endl;
        return -1;
    }

    if (!LoadImage("image/minhgiao.jpg", minhgiao))
    {
        std::cerr << "Failed to load minhgiao image!" << std::endl;
        return -1;
    }

    if (!LoadImage("image/daulinh.png", daulinh))
    {
        std::cerr << "Failed to load daulinh image!" << std::endl;
        return -1;
    }

    if (!LoadImage("image/quaithuong.png", quaithuong))
    {
        std::cerr << "Failed to load quaithuong image!" << std::endl;
        return -1;
    }

    if (!LoadBackGround("image/volam2.jpg", g_background_3))
    {
        std::cerr << "Failed to load quaithuong image!" << std::endl;
        return -1;
    }

    if (!LoadImage("image/lose.png", lose))
    {
        std::cerr << "Failed to load lose image!" << std::endl;
        return -1;
    }
    if (!LoadImage("image/win.png", win))
    {
        std::cerr << "Failed to load win image!" << std::endl;
        return -1;
    }

    bool is_quit = false;
    SDL_StartTextInput();
    while (!is_quit)
    {
        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT)
            {
                is_quit = true;
            }
            else if (g_event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                is_text_box1_selected = false;
                is_text_box2_selected = false;
                is_text_box3_selected = false;
                is_text_box4_selected = false;
                is_text_box5_selected = false;
                is_text_box6_selected = false;
                is_text_box7_selected = false;


                if (use_bg1 && !use_bg2 && !use_bg3 && !use_bg4 )
                {
                    if (x >= 67 && x <= 675 + g_image_button_1.GetWidth() && y >= 350 && y <= 350 + g_image_button_1.GetHeight())
                    {
                        use_bg2 = true;
                        use_bg1 = false;
                        use_bg3 = false;
                        use_bg4 = false;
                    }
                    else if (x >= 675 && x < 675 + g_image_button_2.GetWidth() && y >= 410 && y <= 410 + g_image_button_2.GetHeight())
                    {
                        use_bg2 = false;
                        use_bg1 = false;
                        use_bg3 = true;
                        use_bg4 = false;
                    }
                    else if (x >= 675 && x < 675 + g_image_button_3.GetWidth() && y >= 470 && y <= 470 + g_image_button_3.GetHeight())
                    {
                        use_bg2 = false;
                        use_bg1 = false;
                        use_bg3 = false;
                        use_bg4 = true;
                    }

                    else if (x >= 675 && x <= 675 + g_image_button_4.GetWidth() && y >= 530 && y <= 530 + g_image_button_4.GetHeight())
                    {
                        is_quit = true;
                    }
                }
                else if (use_bg2 && !use_bg1 && !show_single_image && !use_bg3)
                {
                    if (x >= 5 && x <= 5 + b_return.GetWidth() && y >= 595 && y <= 595 + b_return.GetHeight())
                    {
                        use_bg2 = false;
                        use_bg1 = true;
                      
                    }
                    else if (x >= 160 && x <= 160 + conlon.GetWidth() && y >= 10 && y <= 10 + conlon.GetHeight())
                    {
                        show_single_image = true;
                        clicked_image = &conlon;
                        bang = "Con Lon";
                        he = "Tho";
                    }
                    else if (x >= 300 && x <= 300 + thieulam.GetWidth() && y >= 10 && y <= 10 + thieulam.GetHeight())
                    {
                        show_single_image = true;
                        clicked_image = &thieulam;
                        bang = "Thieu Lam";
                        he = "Kim";
                    }
                    else if (x >= 440 && x <= 440 + vodang.GetWidth() && y >= 10 && y <= 10 + vodang.GetHeight())
                    {
                        show_single_image = true;
                        clicked_image = &vodang;
                        bang = "Vo Dang";
                        he = "Tho";
                    }
                    else if (x >= 580 && x <= 580 + ngamy.GetWidth() && y >= 10 && y <= 10 + ngamy.GetHeight())
                    {
                        show_single_image = true;
                        clicked_image = &ngamy;
                        bang = "Nga My";
                        he = "Thuy";
                    }
                    else if (x >= 720 && x <= 720 + ngudoc.GetWidth() && y >= 10 && y <= 10 + ngudoc.GetHeight())
                    {
                        show_single_image = true;
                        clicked_image = &ngudoc;
                        bang = "Ngu Doc";
                        he = "Moc";
                    }
                    else if (x >= 160 && x <= 160 + caibang.GetWidth() && y >= 220 && y <= 220 + caibang.GetHeight())
                    {
                        show_single_image = true;
                        clicked_image = &caibang;
                        bang = "Cai Bang";
                        he = "Hoa";
                    }
                    else if (x >= 300 && x <= 300 + duongmon.GetWidth() && y >= 220 && y <= 220 + duongmon.GetHeight())
                    {
                        show_single_image = true;
                        clicked_image = &duongmon;
                        bang = "Duong Mon";
                        he = "Moc";
                    }
                    else if (x >= 440 && x <= 440 + thiennhan.GetWidth() && y >= 220 && y <= 220 + thiennhan.GetHeight())
                    {
                        show_single_image = true;
                        clicked_image = &thiennhan;
                        bang = "Thien Nhan";
                        he = "Hoa";
                    }
                    else if (x >= 580 && x <= 580 + thuyyen.GetWidth() && y >= 220 && y <= 220 + thuyyen.GetHeight())
                    {
                        show_single_image = true;
                        clicked_image = &thuyyen;
                        bang = "Thuy Yen";
                        he = "Thuy";
                    }
                    else if (x >= 720 && x <= 720 + minhgiao.GetWidth() && y >= 220 && y <= 220 + minhgiao.GetHeight())
                    {
                        show_single_image = true;
                        clicked_image = &minhgiao;
                        bang = "Minh Giao";
                        he = "Kim";
                    }
                    else if (x >= 580 && x <= 580 + daulinh.GetWidth() && y >= 430 && y <= 430 + daulinh.GetHeight())
                    {
                        show_single_image = true;
                        type = "Dau Linh";
                        clicked_image = &daulinh;
                    }
                    else if (x >= 300 && x <= 300 + quaithuong.GetWidth() && y >= 430 && y <= 430 + quaithuong.GetHeight())
                    {
                        show_single_image = true;
                        type = "Quai Thuong";
                        clicked_image = &quaithuong;
                    }
                }
                else if (use_bg2 && show_single_image)
                {
                    if (x >= 5 && x <= 5 + b_return.GetWidth() && y >= 595 && y <= 595 + b_return.GetHeight())
                    {
                        show_single_image = false;
                        clicked_image = nullptr;
                        input_text1 = "";  // Reset input text
                        input_text2 = "";
                        input_text3 = "";  // Reset input text
                        input_text4 = "";
                        input_text5 = "";
                        bang = "";
                        he = "";
                        type = "";

                    }
                    else if (x >= 360 && x <= 560 && y >= clicked_image->GetHeight() + 70 && y <= clicked_image->GetHeight() + 93 && clicked_image != &quaithuong && clicked_image!= &daulinh)
                    {
                        is_text_box1_selected = true;
                    }
                    // Check if click is within the second text box
                    else if (x >= 360 && x <= 560 && y >= clicked_image->GetHeight() + 110 && y <= clicked_image->GetHeight() + 133 && clicked_image != &quaithuong && clicked_image != &daulinh)
                    {
                        is_text_box2_selected = true;
                    }
                    else if (x >= 370 && x <= 570 && y >= clicked_image->GetHeight() + 60 && y <= clicked_image->GetHeight() + 83 )
                    {
                        is_text_box3_selected = true;
                    }
                    // Check if click is within the second text box
                    else if (x >= 370 && x <= 570 && y >= clicked_image->GetHeight() + 100 && y <= clicked_image->GetHeight() + 123 )
                    {
                        is_text_box4_selected = true;
                    }
                    else if (x >= 370 && x <= 570 && y >= clicked_image->GetHeight() + 140 && y <= clicked_image->GetHeight() + 163 )
                    {
                        is_text_box5_selected = true;
                    }
                }

                else if (use_bg3 && !use_bg2 && !use_bg1 && !use_bg4)
                {
                    if (x >= 5 && x <= 5 + b_return.GetWidth() && y >= 595 && y <= 595 + b_return.GetHeight())
                    {
                        use_bg3 = false;
                        use_bg1 = true;
                        use_bg2 = false;
                        use_bg4 = false;

                    }
                }
               
                else if (use_bg4 && !use_bg2 && !use_bg1 && !use_bg3 && !combat_mode)
                {
                    if (x >= 5 && x <= 5 + b_return.GetWidth() && y >= 595 && y <= 595 + b_return.GetHeight())
                    {
                        use_bg3 = false;
                        use_bg1 = true;
                        use_bg2 = false;
                        use_bg4 = false;
                        input_text6 = "";
                        input_text7 = "";
                        combat_mode = false;

                    }
                    else if (x >= 230 && x <= 230 + 32 && y >= 90 && y <= 90 + 25)
                    {
                        is_text_box6_selected = true;
                    }
                    else if (x >= 730 && x <= 730 + 32 && y >= 90 && y <= 90 + 25)
                    {
                        is_text_box7_selected = true;
                    }
                }
                else if (use_bg4 && combat_mode)
                {
                    if (x >= 5 && x <= 5 + b_return.GetWidth() && y >= 595 && y <= 595 + b_return.GetHeight())
                    { 
                        combat_mode = false;
                        vitri.pop_back();
                        vitri.pop_back();
                    }
                }
            }
           else if (g_event.type == SDL_TEXTINPUT)
           {
               if (is_text_box1_selected)
               {
                   input_text1 += g_event.text.text;
               }
               else if (is_text_box2_selected)
               {
                   input_text2 += g_event.text.text;
               }
               else if (is_text_box3_selected)
               {
                   input_text3 += g_event.text.text;
               }
               else if (is_text_box4_selected)
               {
                   input_text4 += g_event.text.text;
               }
               else if (is_text_box5_selected)
               {
                   input_text5 += g_event.text.text;
               }
               else if (is_text_box6_selected)
               {
                   input_text6 += g_event.text.text;
               }
               else if (is_text_box7_selected)
               {
                   input_text7 += g_event.text.text;
               }
            }
           else if (g_event.type == SDL_KEYDOWN)
           {
               if (g_event.key.keysym.sym == SDLK_BACKSPACE)
               {
                   if (is_text_box1_selected && input_text1.length() > 0)
                   {
                       input_text1.pop_back();
                   }
                   else if (is_text_box2_selected && input_text2.length() > 0)
                   {
                       input_text2.pop_back();
                   }
                   else if (is_text_box3_selected && input_text3.length() > 0)
                   {
                       input_text3.pop_back();
                   }
                   else if (is_text_box4_selected && input_text4.length() > 0)
                   {
                       input_text4.pop_back();
                   }
                   else if (is_text_box5_selected && input_text5.length() > 0)
                   {
                       input_text5.pop_back();
                   }
                   else if (is_text_box6_selected && input_text6.length() > 0)
                   {
                       input_text6.pop_back();
                   }
                   else if (is_text_box7_selected && input_text7.length() > 0)
                   {
                       input_text7.pop_back();
                   }
               }
               if (g_event.key.keysym.sym == SDLK_RETURN)
               {
                   if (input_text1 != "" && input_text2 != "")
                   {
                       std::stringstream ss(input_text2);
                       int level;
                       ss >> level;
                       characters.push_back(new Player(input_text1, bang, level));
                       input_text1 = "";
                       input_text2 = "";
                   }
                   else if (input_text3 != "" && input_text4 != "" && input_text5 != "")
                   {
                       std::stringstream ss(input_text4);
                       int level;
                       ss >> level;
                       if (type == "Dau Linh")
                           characters.push_back(new Monster(input_text3, level, true, input_text5));
                       else if (type == "Quai Thuong")
                           characters.push_back(new Monster(input_text3, level, false, input_text5));
                       input_text3 = "";
                       input_text5 = "";
                       input_text4 = "";
                   }

                   else if (input_text6 != "" && input_text7 != "")
                   {
                       std::stringstream ss(input_text6);
                       int local;
                       ss >> local;
                       std::stringstream ss1(input_text7);
                       int local1;
                       ss1 >> local1;
                       if (local > characters.size() || local1 > characters.size())
                       {
                           input_text6 = "";
                           input_text7 = "";
                       }
                       vitri.push_back(local-1);
                       vitri.push_back(local1-1);   
                       combat_mode = true;

                   }
               }
            }
        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);

        if (use_bg2 && !use_bg1 && !use_bg3 && !use_bg4)
        {
            g_background_2.Render(g_screen, 0, 0);
            b_return.Render(g_screen, 5, 595);

            if (!show_single_image)
            {
                int x = 100;
                conlon.Render(g_screen,160, 10);
                thieulam.Render(g_screen, 300, 10);
                vodang.Render(g_screen, 440, 10);
                ngamy.Render(g_screen, 580, 10);
                ngudoc.Render(g_screen, 720, 10);
                caibang.Render(g_screen, 160, 220);
                duongmon.Render(g_screen, 300,220);
                thiennhan.Render(g_screen, 440, 220);
                thuyyen.Render(g_screen, 580, 220);
                minhgiao.Render(g_screen, 720, 220);
                daulinh.Render(g_screen, 580, 430);
                quaithuong.Render(g_screen, 300 , 430);
            }
            else
            {
                if (clicked_image && clicked_image != &quaithuong && clicked_image != &daulinh)
                {
                    clicked_image->Render(g_screen,160,200);
                 
                    RenderText("NAME: ", 300, clicked_image->GetHeight() + 70,g_font_18);
                    RenderText("LEVEL: ", 300, clicked_image->GetHeight() +110,g_font_18);
                    SDL_Rect text_box = { 360, clicked_image->GetHeight() + 70, 200, 23 };
                    SDL_Rect text_box1 = { 360, clicked_image->GetHeight() + 110, 200, 23};
                    SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);  // Màu trắng cho hộp văn bản
                    SDL_RenderFillRect(g_screen, &text_box);
                    SDL_RenderFillRect(g_screen, &text_box1);

                    RenderText(input_text1, 365, clicked_image->GetHeight() + 70,g_font_18);
                    RenderText(input_text2, 365, clicked_image->GetHeight() + 110,g_font_18);

                }
                else
                {
                    clicked_image->Render(g_screen, 160, 200);
                    RenderText("NAME: ", 310, clicked_image->GetHeight() + 60,g_font_18);
                    RenderText("LEVEL: ", 310, clicked_image->GetHeight() + 100,g_font_18);
                    RenderText("TYPE: ", 310, clicked_image->GetHeight() + 140,g_font_18);
                    SDL_Rect text_box = { 370, clicked_image->GetHeight() + 60, 200, 23 };
                    SDL_Rect text_box1 = { 370, clicked_image->GetHeight() + 100, 200, 23 };
                    SDL_Rect text_box2 = { 370, clicked_image->GetHeight() + 140, 200, 23 };

                    SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);  // Màu trắng cho hộp văn bản
                    SDL_RenderFillRect(g_screen, &text_box);
                    SDL_RenderFillRect(g_screen, &text_box1);
                    SDL_RenderFillRect(g_screen, &text_box2);

                    RenderText(input_text3, 375, clicked_image->GetHeight() + 60,g_font_18);
                    RenderText(input_text4, 375, clicked_image->GetHeight() + 100,g_font_18);
                    RenderText(input_text5, 375, clicked_image->GetHeight() + 140,g_font_18);
                }
            }
        }
        else if (use_bg3 && !use_bg2 && !use_bg1 && !use_bg4)
        {
            g_background_3.Render(g_screen, 0, 0);
            b_return.Render(g_screen, 5, 595);
            sort(characters.begin(), characters.end(), compareNhanVat);
            int y_offset = 15;
            int x_offset = 30;
            int y_offset_text = caibang.GetHeight() + 15;
            int quantity = 0;
            int num = 0;
            for (NhanVat* k : characters)
            {
                if (quantity == 7)
                {
                    x_offset = 30;
                    y_offset += caibang.GetHeight() + 15 + 10;
                    y_offset_text += caibang.GetHeight() + 10 + 15;
                    quantity = 0;
                    num++;
                }
                if (num == 2)
                {
                    x_offset += caibang.GetWidth() + 10;
                    num = 0;
                }
                Player* player = dynamic_cast<Player*>(k);
                if (player)
                {
                    if (player->getSect() == "Cai Bang")
                    {
                        caibang.Render(g_screen, x_offset, y_offset);
                        RenderText(player->getName() + "-" + to_string(player->getLevel()), x_offset + 10, y_offset_text, g_font_13);
                        x_offset += caibang.GetWidth() + 10;
                    }
                    if (player->getSect() == "Con Lon")
                    {
                        conlon.Render(g_screen, x_offset, y_offset);
                        RenderText(player->getName() + "-" + to_string(player->getLevel()), x_offset + 10, y_offset_text, g_font_13);
                        x_offset += caibang.GetWidth() + 10;

                    }
                    if (player->getSect() == "Thieu Lam")
                    {
                        thieulam.Render(g_screen, x_offset, y_offset);
                        RenderText(player->getName() + "-" + to_string(player->getLevel()), x_offset + 10, y_offset_text, g_font_13);
                        x_offset += caibang.GetWidth() + 10;


                    }
                    if (player->getSect() == "Vo Dang")
                    {
                        vodang.Render(g_screen, x_offset, y_offset);
                        RenderText(player->getName() + "-" + to_string(player->getLevel()), x_offset + 10, y_offset_text, g_font_13);
                        x_offset += caibang.GetWidth() + 10;


                    }
                    if (player->getSect() == "Nga My")
                    {
                        ngamy.Render(g_screen, x_offset, y_offset);
                        RenderText(player->getName() + "-" + to_string(player->getLevel()), x_offset + 10, y_offset_text, g_font_13);
                        x_offset += caibang.GetWidth() + 10;


                    }
                    if (player->getSect() == "Ngu Doc")
                    {
                        ngudoc.Render(g_screen, x_offset, y_offset);
                        RenderText(player->getName() + "-" + to_string(player->getLevel()), x_offset + 10, y_offset_text, g_font_13);
                        x_offset += caibang.GetWidth() + 10;

                    }
                    if (player->getSect() == "Duong Mon")
                    {
                        duongmon.Render(g_screen, x_offset, y_offset);
                        RenderText(player->getName() + "-" + to_string(player->getLevel()), x_offset + 10, y_offset_text, g_font_13);
                        x_offset += caibang.GetWidth() + 10;


                    }
                    if (player->getSect() == "Thien Nhan")
                    {
                        thiennhan.Render(g_screen, x_offset, y_offset);
                        RenderText(player->getName() + "-" + to_string(player->getLevel()), x_offset + 10, y_offset_text, g_font_13);
                        x_offset += caibang.GetWidth() + 10;


                    }
                    if (player->getSect() == "Thuy Yen")
                    {
                        thuyyen.Render(g_screen, x_offset, y_offset);
                        RenderText(player->getName() + "-" + to_string(player->getLevel()), x_offset + 10, y_offset_text, g_font_13);
                        x_offset += caibang.GetWidth() + 10;


                    }
                    if (player->getSect() == "Minh Giao")
                    {
                        minhgiao.Render(g_screen, x_offset, y_offset);
                        RenderText(player->getName() + "-" + to_string(player->getLevel()), x_offset + 10, y_offset_text, g_font_13);
                        x_offset += caibang.GetWidth() + 10;


                    }
                }
                else
                {
                    Monster* monster = dynamic_cast<Monster*>(k);
                    if (monster->isBossMonster() == true)
                    {
                        daulinh.Render(g_screen, x_offset, y_offset);
                        RenderText(monster->getName() + "-" + to_string(monster->getLevel()) + " - " + monster->getElement(), x_offset + 5, y_offset_text, g_font_13);
                        x_offset += caibang.GetWidth() + 10;
                    }
                    else
                    {
                        quaithuong.Render(g_screen, x_offset, y_offset);
                        RenderText(monster->getName() + "-" + to_string(monster->getLevel()) + " - " + monster->getElement(), x_offset + 5, y_offset_text, g_font_13);
                        x_offset += caibang.GetWidth() + 10;

                    }
                }
                quantity++;
            }

        }
        else if (use_bg4 && !use_bg2 && !use_bg1 && !use_bg3)
        {
            sort(characters.begin(), characters.end(), compareNhanVat);
            
            g_background_3.Render(g_screen, 0, 0);
            b_return.Render(g_screen, 5, 595);
            RenderText("Character 1", 200, 60, g_font_18);
            RenderText("Character 2", 700, 60, g_font_18);
            SDL_Rect text_box = { 230, 90, 32,25 };
            SDL_Rect text_box1 = { 730,90, 32,25 };
            SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
            SDL_RenderFillRect(g_screen, &text_box);
            SDL_RenderFillRect(g_screen, &text_box1);

            RenderText(input_text6, 235, 95, g_font_18);
            RenderText(input_text7, 735, 95, g_font_18);
            if (combat_mode)
            {
                Player* player = dynamic_cast<Player*>(characters[vitri[0]]);
                Player* player1 = dynamic_cast<Player*>(characters[vitri[1]]);
                NhanVat* A = characters[vitri[0]];
                NhanVat* B = characters[vitri[1]];
                string AtoB = to_string(calculateDamage(*A, *B));
                string BtoA = to_string(calculateDamage(*B, *A));

                RenderText("ATK: " + AtoB, 328, 250, g_font_18);
                RenderText("ATK: " + BtoA, 818, 250, g_font_18);

                if (calculateDamage(*A,*B) > calculateDamage(*B,*A))
                {
                    win.Render(g_screen, 140, 375);
                    lose.Render(g_screen, 645, 375);

                }
                else if (calculateDamage(*A, *B) < calculateDamage(*B, *A))
                {
                    win.Render(g_screen, 645, 375);
                    lose.Render(g_screen, 140, 375);
                }
                if (player)
                {
                    if (player->getSect() == "Con Lon")
                    {
                        conlon.Render(g_screen, 180, 150);
                        RenderText(player->getName() + "-" + to_string(player->getLevel()), 185, conlon.GetHeight() + 150, g_font_13);
                    }
                    if (player->getSect() == "Cai Bang")
                    {
                        caibang.Render(g_screen, 180, 150);
                        RenderText(player->getName() + "-" + to_string(player->getLevel()), 185, conlon.GetHeight() + 150, g_font_13);
                    }
                    if (player->getSect() == "Thieu Lam")
                    {
                        thieulam.Render(g_screen, 180, 150);
                        RenderText(player->getName() + "-" + to_string(player->getLevel()), 185, conlon.GetHeight() + 150, g_font_13);
                    }
                    if (player->getSect() == "Nga My")
                    {
                        ngamy.Render(g_screen, 180, 150);
                        RenderText(player->getName() + "-" + to_string(player->getLevel()), 185, conlon.GetHeight() + 150, g_font_13);
                    }
                    if (player->getSect() == "Vo Dang")
                    {
                        vodang.Render(g_screen, 180, 150);
                        RenderText(player->getName() + "-" + to_string(player->getLevel()), 185, conlon.GetHeight() + 150, g_font_13);
                    }
                    if (player->getSect() == "Ngu Doc")
                    {
                        ngudoc.Render(g_screen, 180, 150);
                        RenderText(player->getName() + "-" + to_string(player->getLevel()), 185, conlon.GetHeight() + 150, g_font_13);
                    }
                    if (player->getSect() == "Duong Mon")
                    {
                        duongmon.Render(g_screen, 180, 150);
                        RenderText(player->getName() + "-" + to_string(player->getLevel()), 185, conlon.GetHeight() + 150, g_font_13);
                    }
                    if (player->getSect() == "Thien Nhan")
                    {
                        thiennhan.Render(g_screen, 180, 150);
                        RenderText(player->getName() + "-" + to_string(player->getLevel()), 185, conlon.GetHeight() + 150, g_font_13);
                    }
                    if (player->getSect() == "Thuy Yen")
                    {
                        thuyyen.Render(g_screen, 180, 150);
                        RenderText(player->getName() + "-" + to_string(player->getLevel()), 185, conlon.GetHeight() + 150, g_font_13);
                    }
                    if (player->getSect() == "Minh Giao")
                    {
                        minhgiao.Render(g_screen, 180, 150);
                        RenderText(player->getName() + "-" + to_string(player->getLevel()), 185, conlon.GetHeight() + 150, g_font_13);
                    }
                }
                      
                else
                {
                    Monster* monster = dynamic_cast<Monster*>(characters[vitri[0]]);
                    if (monster->isBossMonster())
                    {
                        daulinh.Render(g_screen, 180, 150);
                        RenderText(monster->getName() + "-" + to_string(monster->getLevel()) + "-" +monster->getElement(), 185, conlon.GetHeight() + 150, g_font_13);
                    }
                    else
                    {
                        quaithuong.Render(g_screen, 180, 150);
                        RenderText(monster->getName() + "-" + to_string(monster->getLevel()) + "-" + monster->getElement(), 185, conlon.GetHeight() + 150, g_font_13);
                    }
                }
                
                if (player1)
                {
                    if (player1->getSect() == "Con Lon")
                    {
                        conlon.Render(g_screen, 680, 150);
                        RenderText(player1->getName() + "-" + to_string(player1->getLevel()), 685, conlon.GetHeight() + 150, g_font_13);
                    }
                    if (player1->getSect() == "Cai Bang")
                    {
                        caibang.Render(g_screen, 680, 150);
                        RenderText(player1->getName() + "-" + to_string(player1->getLevel()), 685, conlon.GetHeight() + 150, g_font_13);
                    }
                    if (player1->getSect() == "Thieu Lam")
                    {
                        thieulam.Render(g_screen, 680, 150);
                        RenderText(player1->getName() + "-" + to_string(player1->getLevel()), 685, conlon.GetHeight() + 150, g_font_13);
                    }
                    if (player1->getSect() == "Nga My")
                    {
                        ngamy.Render(g_screen, 680, 150);
                        RenderText(player1->getName() + "-" + to_string(player1->getLevel()),685, conlon.GetHeight() + 150, g_font_13);
                    }
                    if (player1->getSect() == "Vo Dang")
                    {
                        vodang.Render(g_screen, 680, 150);
                        RenderText(player1->getName() + "-" + to_string(player1->getLevel()), 685, conlon.GetHeight() + 150, g_font_13);
                    }
                    if (player1->getSect() == "Ngu Doc")
                    {
                        ngudoc.Render(g_screen, 680, 150);
                        RenderText(player1->getName() + "-" + to_string(player1->getLevel()), 685, conlon.GetHeight() + 150, g_font_13);
                    }
                    if (player1->getSect() == "Duong Mon")
                    {
                        duongmon.Render(g_screen, 680, 150);
                        RenderText(player1->getName() + "-" + to_string(player1->getLevel()), 685, conlon.GetHeight() + 150, g_font_13);
                    }
                    if (player1->getSect() == "Thien Nhan")
                    {
                        thiennhan.Render(g_screen, 680, 150);
                        RenderText(player1->getName() + "-" + to_string(player1->getLevel()), 685, conlon.GetHeight() + 150, g_font_13);
                    }
                    if (player1->getSect() == "Thuy Yen")
                    {
                        thuyyen.Render(g_screen, 680, 150);
                        RenderText(player1->getName() + "-" + to_string(player1->getLevel()), 685, conlon.GetHeight() + 150, g_font_13);
                    }
                    if (player1->getSect() == "Minh Giao")
                    {
                        minhgiao.Render(g_screen, 680, 150);
                        RenderText(player1->getName() + "-" + to_string(player1->getLevel()), 685, conlon.GetHeight() + 150, g_font_13);
                    }
                }
                
                else
                {
                    Monster* monster = dynamic_cast<Monster*>(characters[vitri[1]]);
                    if (monster->isBossMonster())
                    {
                        daulinh.Render(g_screen, 680, 150);
                        RenderText(monster->getName() + "-" + to_string(monster->getLevel()) + "-" + monster->getElement(), 685, conlon.GetHeight() + 150, g_font_13);
                    }
                    else
                    {
                        quaithuong.Render(g_screen, 680, 150);
                        RenderText(monster->getName() + "-" + to_string(monster->getLevel()) +  "-" +monster->getElement(), 685, conlon.GetHeight() + 150, g_font_13);
                    }
                }
            }
        }
        else if (use_bg1 && !use_bg2 && !use_bg3 && !use_bg4)
        {
            g_background.Render(g_screen, 0, 0);
            g_image_button_1.Render(g_screen, 675, 350);
            g_image_button_2.Render(g_screen, 675, 410);
            g_image_button_3.Render(g_screen, 675, 470);
            g_image_button_4.Render(g_screen, 675, 530);
        }
        SDL_RenderPresent(g_screen);
    }
    SDL_StopTextInput();
    close();
    return 0;
}