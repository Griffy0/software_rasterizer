# Documentation


## Types

### Render-Types

#### **vec2** (**float** x, **float** y, **float** z)
- Used for screen-space rendering and mouse positions
- 
    <details open>
    <summary> Methods </summary>

    **\*** (**vec2** this, **float** scalar)
    - Non-Mutative
    - $$vec3(x,y,z) = (this.x \times scalar, this.y \times scalar, this.z \times scalar)$$
    <br>

    **/** (**vec3** this, **float** scalar)
    - Non-Mutative
    - $$vec3(x,y,z) = (\frac{this.x}{scalar}, \frac{this.y}{scalar}, \frac{this.z}{scalar})$$
    <br>
    </details>

<br>


#### **vec3** (**float** x, **float** y, **float** z)
- Used for storing positions and Euler Angles
- Limited floating point precision can lead to discrepencies in calculations
- 
    <details open>
    <summary> Methods </summary>
    
    ### vec3 x vec3
    **\+** (**vec3** this, **vec3** b)
    - Adds each x, y, and z value into a new **vec3**
    - $$vec3(x,y,z) = (this.x+b.x, this.y+b.y, this.z+b.z)$$
    <br>

    **\-** (**vec3** this, **vec3** b)
    - Subtracts each x, y, and z value into a new **vec3**
    - $$vec3(x,y,z) = (this.x-b.x, this.y-b.y, this.z-b.z)$$
    <br>

    **\*** (**vec3** this, **vec3** b)
    - Multiplies each x, y, and z value into a new **vec3**
    - $$vec3(x,y,z) = (this.x \times b.x, this.y \times b.y, this.z \times b.z)$$
    <br>
        
    **/** (**vec3** this, **vec3** b)
    - Divides each x, y, and z value into a new **vec3**
    - $$vec3(x,y,z) = (\frac{this.x}{b.x}, \frac{this.y}{b.y}, \frac{this.z}{b.z})$$
    
    <br>

    ### vec3 x float
    **\+** (**vec3** this, **float** scalar)
    - Creates a new **vec3** with x, y, and z incremented by float
    - $$vec3(x,y,z) = (this.x+scalar, this.y+scalar, this.z+scalar)$$
    <br>

    **\-** (**vec3** this, **float** scalar)
    - Creates a new **vec3** with x, y, and z decremented by float
    - $$vec3(x,y,z) = (this.x-scalar, this.y-scalar, this.z-scalar)$$
    <br>

    **\*** (**vec3** this, **float** scalar)
    - Non-Mutative
    - $$vec3(x,y,z) = (this.x \times scalar, this.y \times scalar, this.z \times scalar)$$
    <br>

    **/** (**vec3** this, **float** scalar)
    - Non-Mutative
    - $$vec3(x,y,z) = (\frac{this.x}{scalar}, \frac{this.y}{scalar}, \frac{this.z}{scalar})$$

    <br>

    ### Other Methods

    float **magnitude**()
    - Returns the the length of the vector using the Pythagorean Theorum
    - $$magnitude = \sqrt{x^2 + y^2 + z^2}$$
    <br>

    void **normalise**()
    - Normalises the vector
    - Mutative
    - $$this = (\frac{this.x}{magnitude}, \frac{this.y}{magnitude}, \frac{this.z}{magnitude})$$
    <br>

    (string)
    - Returns the vector as a string in format ({x}, {y}, {z})
    <br>
    </details>

<br>

#### **tri3** (vec3 a, vec3 b, vec3 c)
- Stores a triangle with three vertices in the 3rd dimension
- Used for screen space triangles too, **z** of each vertex is used for **cull space w**
- 
    <details open>
    <summary> Methods </summary>


    </details>

#### **RenderTri** ()
- info
- 
    <details open>
    <summary> Methods </summary>


    </details>

<br>



#### **Type** (ConstructorVars)
- info
- 
    <details open>
    <summary> Methods </summary>


    </details>

<br>

