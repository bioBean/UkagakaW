﻿#region Statement

/*
* ---------------------Statement---------------------
* Copyright(c) 2018-2018 Bio-Electric Studio
* All Right Reserved
* This C# source file is for the Application of Project Ukagaka_W.
* You are not allowed to copy any code from here without permission.
*
* File: Ukagaka
* Author: 47339
* Create On: 2018/6/29 16:57:02
*
* Overall Description:
* None
*
* Update Description:
* None
*
*
* ---------------------------------------------------
*/

#endregion

using System;
using System.Collections.Generic;

using UkagakaW.Render;

namespace UkagakaW.Core
{
    class Ukagaka
    {
        public UkagakaRenderer renderer;

        public IntPtr hWND;

        public Ukagaka(IntPtr hWnd)
        {
            this.hWND = hWnd;
        }

    }
}
