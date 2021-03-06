﻿// Copyright(c) 2018 Seth Ballantyne <seth.ballantyne@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files(the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and / or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace PLeD
{
    /// <summary>
    /// Represents the grid that can be used a guide when a drawing bricks.
    /// </summary>
    internal class Grid : IDisposable
    {
        // the width and height of each cell in the grid.
        // It (obviously) really represents the width and height of each brick.
        int cellWidth;
        int cellHeight;

        // the total width and height in pixels of the grids width and height
        int gridWidth;
        int gridHeight;

        Color gridColour;

        // used to draw the grid.
        Pen gridPen;

        // true if the grid is visible and being rendered, otherwise false.
        bool visible;

        bool disposed = false;
        
        protected virtual void Dispose(bool disposing)
        {
            if(disposed)
            {
                return;
            }

            if(disposing && gridPen != null)
            {
                gridPen.Dispose();
            }

            disposed = true;
        }

        /// <summary>
        /// Initialies a new instance of Grid with the specified dimensions, colour and visibility.
        /// </summary>
        /// <param name="width">the width of the grid in pixels.</param>
        /// <param name="height">the height of the grid in pixels.</param>
        /// <param name="colour">the desired colour of the grid.</param>
        /// <param name="visible"><b>true</b> to render the grid, otherwise <b>false</b>.</param>
        public Grid(int width, int height, Color colour, bool visible)
        {
            this.gridWidth = width;
            this.gridHeight = height;
            this.gridColour = colour;
            this.visible = visible;

            this.gridPen = new Pen(colour);
        }

        public void Dispose()
        {
            Dispose(true);

            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Draws the grid onto the specified graphics context. 
        /// </summary>
        /// <param name="renderTarget">the graphics context to draw the grid on to.</param>
        /// <exception cref="System.ArgumentNullException"><i>renderTarget</i> is <b>null</b>.</exception>
        public void Render(Graphics renderTarget)
        {
            // no point in rendering if the user can't see it.
            if(!this.visible)
            {
                return;
            }

            // horizontal lines
            for (int i = 0; i <= this.gridHeight; i += cellHeight)
            {
                renderTarget.DrawLine(this.gridPen, 0, i, this.gridWidth, i);
            }

            // vertical lines
            for(int i = 0; i < this.gridWidth; i+= cellWidth)
            {
                renderTarget.DrawLine(this.gridPen, i, 0, i, this.gridHeight);
            }

            renderTarget.DrawLine(this.gridPen, 1023, 0, 1023, 460);
            
        }

        /// <summary>
        /// Gets or sets the height of the cells within the grid in pixels.
        /// </summary>
        public int CellHeight
        {
            get
            {
                return this.cellHeight;
            }

            set
            {
                this.cellHeight = value;
            }
        }

        /// <summary>
        /// Gets or sets the width of the cells within the grid in pixels.
        /// </summary>
        public int CellWidth
        {
            get
            {
                return this.cellWidth;
            }
            set
            {
                this.cellWidth = value;
            }
        }

        /// <summary>
        /// Gets or sets the colour of the grid.
        /// When assigning a value, the settings file will be updated with the new value.
        /// </summary>
        public Color Colour
        {
            get
            {
                return this.gridColour;
            }
            set
            {
                this.gridColour = value;
                gridPen.Color = this.gridColour;
               
                Properties.Settings.Default.GridColour = this.gridColour;
                Properties.Settings.Default.Save();
            }
        }

        /// <summary>
        /// Gets or sets the total height of the grid in pixels.
        /// </summary>
        public int Height
        {
            get
            {
                return this.gridHeight;
            }
            set
            {
                this.gridHeight = value;
            }
        }

        /// <summary>
        /// Gets or sets the total width of the grid in pixels.
        /// </summary>
        public int Width
        {
            get
            {
                return this.gridWidth;
            }
            set
            {
                this.gridWidth = value;
            }
        }
        /// <summary>
        /// Gets or sets whether the grid is visible.
        /// When assigning a value, the settings file will be updated with the new value.
        /// </summary>
        public bool Visible
        {
            get
            {
                return this.visible;
            }
            set
            {
                this.visible = value;
                Properties.Settings.Default.GridIsVisible = this.visible;
                Properties.Settings.Default.Save();
            }
        }
    }
}
