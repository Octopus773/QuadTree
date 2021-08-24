// taken from https://editor.p5js.org/Aslatas/sketches/bQwvi43Uf
// and modified to print code for test on quadtrees


/* This is a demo/prototype of a pretty standard Quad-Tree data structure.
 * In this example, it's used to speed up collision detection for a grid of
 * particles. This example is mostly trivial, and ignores some complexities
 * that arise when particles intersect more than one tree node.
 *
 * The amount of speed-up from the tree varies depending on the sparseness and
 * configuration. For example, the worst case performance occurs when all
 * particles overlap, and requires the same number of collision checks as the
 * naive approach, with the added cost of constructing and searching the tree.
 *
 * For a relatively balanced tree, however, the quadtree massively reduces the
 * number of particles that need tested for collision, at the (comparatively
 * small) cost of constructing and searching the tree each frame.
 *
 * This implementation stores points only in leaf nodes, which slightly increases
 * the cost of successive insertions, but cheapens lookup. In most configurations,
 * between n and 2n insertions are performed per frame.
 *
 * It's worth noting that for very sparse trees, compression strategies can
 * be used to avoid storing empty nodes - these aren't implemented in this example.
 *
 * Pseudocode for this implementation is from:
 * https://en.wikipedia.org/wiki/Quadtree#Pseudo_code
 *
 * Author: Matthew McLaurin
 * Date: 2/23/2019
 * Copyright: This source code is offered under the open-source MIT license,
 * available here: https://opensource.org/licenses/MIT
 */

var nameIt =0;

// Friendly error checking has a big performance cost, but feel free to turn it off
// if you are modifying the code.
p5.disableFriendlyErrors = true;

// Configuration for colors, canvas, and text size.
const BACKGROUND_COLOR = "#333333"; // Color of the background.
const PARTICLE_COLOR = "#ffffff"; // Current particle color.
const COLLIDE_COLOR = "#5555ff"; // Current colliding color.
const FULL_COLOR = "#aaaaaa"; // Current full node color.
const CANVAS_SIZE = 512; // Size of the canvas, use -1 for fullscreen.
const TEXT_SIZE = 14; // On-canvas text size.

// The array of particles.
let particles = [];

// Performance counters.
let test_count = 0; // Collision test counter.
let insert_count = 0; // Insertion counter.
let node_count = 0; // Tree node counter;

// UI controls.
let particle_speed_input; // Slider that controls particle speed.
let particle_size_input; // Slider that controls particle size.
let particle_count_input; // Slider that contorls particle count.
let bin_size_input; // Field that controls the tree node size.
let use_tree_input; // Checkbox that controls if the tree is used.
let draw_tree_input; // Checkbox that controls if the tree is drawn.

/*--------------------------------------------------------------------*
 * A bounding square, defined by upper left corner and side length.   *
 * Can check for intersection with points and other bounding squares. *
 *--------------------------------------------------------------------*/
class SquareBounds {
    constructor(left, top, side_length) {
        this.top = top;
        this.left = left;
        this.right = left + side_length;
        this.bottom = top + side_length;
    }

    // Returns true if the point is inside the boundary, or false otherwise.
    // Note that the top/left edges are inclusive, and bottom/right are exclusive.
    Contains(p) {
        if (p.x < this.left || p.x >= this.right) return false;
        if (p.y < this.top || p.y >= this.bottom) return false;
        return true;
    }

    // Returns true if this boundary intersects the other, false otherwise.
    Intersects(b) {
        if (this.right < b.left || b.right < this.left) return false;
        if (this.bottom < b.top || b.bottom < this.top) return false;
        return true;
    }

    // Draws the boundary, given a fullness value between zero and one.
    Draw(fullness, name = "") {
        strokeWeight(2);
        stroke(BACKGROUND_COLOR);
        fill(lerpColor(color(BACKGROUND_COLOR), color(FULL_COLOR), fullness));
        let size = this.right - this.left;
        rect(this.left, this.top, size, size);
        if (name== "") return;
        textSize(TEXT_SIZE);
        strokeWeight(1);
        stroke(0);
        fill(255);
        text(name, this.left, this.top);
    }
}

/*--------------------------------------------------------------*
 * One node in a quadtree. The root represents the whole tree.  *
 * Maintains a square bounds, and an array of contained points. *
 *--------------------------------------------------------------*/
class TreeNode {
    constructor(bounds) {
        this.bounds = bounds;
        this.points = [];
        this.is_leaf = true;
        node_count++;
        this.name = nameIt++;
    }

    printElts()
    {
        console.log("node name: " + this.name);
        for (let p of this.points) {
            console.log("points.emplace_back(std::make_shared<QuadTree::Tests::Point>("+p.x+ ", "+p.y+ ", \"" + p.name + "\"));");
        }
    }

    // Inserts a point into the tree rooted at this node.
    // Returns true if successful, or false otherwise.
    Insert(point) {
        if (!this.bounds.Contains(point)) return false;

        if (this.points.length < bin_size_input.value() && this.is_leaf) {
            this.points.push(point);
            insert_count++;
            return true;
        }

        if (this.is_leaf) this.Subdivide();
        if (this.nw.Insert(point)) return true;
        if (this.ne.Insert(point)) return true;
        if (this.sw.Insert(point)) return true;
        if (this.se.Insert(point)) return true;

        // This shouldn't ever happen if the children fully occupy the parent bounds.
        return false;
    }

    // Adds children to a node, and migrates it's points into the children.
    Subdivide() {
        // Create four children with the new side length, to span the parent boundary.
        let w = (this.bounds.right - this.bounds.left) * 0.5;
        let x = this.bounds.left + w;
        let y = this.bounds.top + w;

        this.nw = new TreeNode(new SquareBounds(this.bounds.left, this.bounds.top, w));
        this.ne = new TreeNode(new SquareBounds(x, this.bounds.top, w));
        this.sw = new TreeNode(new SquareBounds(this.bounds.left, y, w));
        this.se = new TreeNode(new SquareBounds(x, y, w));

        // Mark this node as a non-leaf, and re-insert children.
        this.is_leaf = false;
        for (let p of this.points) this.Insert(p);
        this.points = [];
    }

    // Gets a list of points from this node contained in a given boundary.
    QueryRange(bounds) {
        let points_in_range = [];

        // If the boundary doesn't intersect this node, return the empty list.
        if (!this.bounds.Intersects(bounds)) return points_in_range;

        // Otherwise, add any contained points.
        for (let p of this.points)
            if (bounds.Contains(p)) points_in_range.push(p);

        // If this was a leaf, go ahead and return, as there are no children to search.
        if (this.is_leaf) return points_in_range;

        // Otherwise, Recursively add any points from children.
        points_in_range = points_in_range.concat(this.nw.QueryRange(bounds));
        points_in_range = points_in_range.concat(this.ne.QueryRange(bounds));
        points_in_range = points_in_range.concat(this.sw.QueryRange(bounds));
        points_in_range = points_in_range.concat(this.se.QueryRange(bounds));
        return points_in_range;
    }

    // Recursively draw this node's boundary and children's boundaries.
    Draw() {
        if (this.is_leaf) {
            this.bounds.Draw(this.points.length / bin_size_input.value(), this.name);
            this.printElts();
        }
        else {
            this.nw.Draw();
            this.ne.Draw();
            this.sw.Draw();
            this.se.Draw();
        }
    }
}

/*---------------------------------------------------------*
 * A simple particle, with a position, velocity, and size. *
 *---------------------------------------------------------*/
class Particle {

    // Spawns the particle with random position, angle, and speed.
    constructor() {
        this.x = random(0, width);
        this.y = random(0, height);
        this.speed = random(1.0);
        this.is_colliding = false;

        let angle = random(TWO_PI);
        this.v_x = cos(angle);
        this.v_y = sin(angle);
        this.name = nameIt++;
    }

    // Applies velocity, and checks for collisions, using the tree if supplied.
    Update(tree) {
        let size = particle_size_input.value();
        let speed = particle_speed_input.value();

        // Only update position if un-paused.
        this.x += this.v_x * this.speed * speed;
        this.y += this.v_y * this.speed * speed;

        this.is_colliding = false;
        let points_to_check; // List of points to compare for collisions.

        // Search the tree if available, otherwise search all particles.
        if (tree) {
            // Create bounds to check. Because max velocity is known, this can be small.
            // Should be as small as possible to reduce the number of nodes searched.
            let w = 2 * (size + speed);
            let check_range = new SquareBounds(this.x - w, this.y - w, w * 2);
            points_to_check = tree.QueryRange(check_range);
        } else {
            points_to_check = particles;
        }

        // Check the list of particles for collisions.
        for (let p of points_to_check) {
            if (p == this) continue;
            ++test_count;

            // Compute center distance of the two points.
            let dx = p.x - this.x;
            let dy = p.y - this.y;
            let distance_squared = (dx * dx) + (dy * dy);

            // If the particles are closer than two radii, they're colliding.
            if (distance_squared < size * size * 4) {
                this.is_colliding = true;
                break;
            }
        }

        // Wrap around the canvas edges if we move past them.
        if (this.x < 0) this.x = (this.x % width) + width;
        if (this.x >= width) this.x %= width;
        if (this.y < 0) this.y = (this.y % height) + height;
        if (this.y >= height) this.y %= height;
    }

    // Draws the particle, picking a color based on whether we are colliding.
    Draw() {
        noStroke();
        fill((this.is_colliding) ? COLLIDE_COLOR : PARTICLE_COLOR);
        circle(this.x, this.y, particle_size_input.value());
    }
}

// Creates the canvas and inputs, and spawns particles.
function setup() {

    let size = (CANVAS_SIZE > 0) ? CANVAS_SIZE : min(windowWidth, windowHeight);
    let canvas = createCanvas(size, size).style("display", "block");
    if (CANVAS_SIZE > 0) canvas.style("border-radius", "5%");
    canvas.mousePressed(() => {
        let p = new Particle();
        p.x = mouseX;
        p.y = mouseY;
        particles.push(p);
        particle_count_input.value(parseInt(particle_count_input.value()) + 1);
    });

    let controls = createDiv();
    controls.style("display", "flex");
    controls.style("width", size + "px");

    let slot = createDiv().parent(controls);
    slot.style("flex", "50%");
    slot.style("flex-direction", "column");
    slot.style("justify-content", "space-around");
    particle_size_input = createSlider(1, 32, 2, 0).parent(slot);

    let label = createSpan(" Particle Size").parent(slot);

    slot = createDiv().parent(controls);
    slot.style("flex", "50%");
    slot.style("flex-direction", "column");
    slot.style("justify-content", "space-around");
    particle_speed_input = createSlider(0, 16, 2, 0).parent(slot);
    label = createSpan(" Particle Speed").parent(slot);

    particle_count_input = createInput("30", "number");
    particle_count_input.attribute("min", "0");
    label = createSpan(" Particle Count");

    bin_size_input = createInput("4", "number");
    bin_size_input.attribute("min", "1");
    label = createSpan(" Bin Size");

    use_tree_input = createCheckbox("Use Tree", true);
    draw_tree_input = createCheckbox("Draw Tree", true);

    // Disable the tree drawing checkbox if we aren't using the quadtree.
    use_tree_input.changed(() => {
        if (use_tree_input.checked()) draw_tree_input.removeAttribute("disabled");
        else draw_tree_input.attribute("disabled", true);
    });

    // Initialize the particles.
    particles = [];
    let count = particle_count_input.value();
    for (let i = 0; i < count; ++i) particles.push(new Particle());
}

// Runs every frame. Creates and fills the tree with particles, then re-draws.
function draw() {
    console.log("start");
    // Clear the frame and reset the global counters.
    background(BACKGROUND_COLOR)
    test_count = 0;
    insert_count = 0;
    node_count = 0;

    // If the particle count has changed since last frame, spawn or remove some accordingly.
    let count = particle_count_input.value();
    while (particles.length < count) particles.push(new Particle());
    if (particles.length > count) particles.length = count;

    nameIt = 0;
    // Re-create and populate the tree, if enabled.
    let tree = null;
    if (use_tree_input.checked()) {
        tree = new TreeNode(new SquareBounds(0, 0, max(width, height)));
        for (let p of particles) tree.Insert(p);
        if (draw_tree_input.checked()) tree.Draw();
    }

    // Update and draw the particles.
    for (let p of particles) p.Update(tree);
    for (let p of particles) p.Draw();

    // Draw the performance counters.
    textSize(TEXT_SIZE);
    strokeWeight(1);
    stroke(0);
    fill(255);
    text('Total Collision Tests: ' + test_count, 10, TEXT_SIZE + 10);
    text('Tests Per Particle: ' + (test_count / particles.length).toFixed(2),
        10, (TEXT_SIZE + 10) * 2);
    if (use_tree_input.checked()) {
        text("Tree Insertions: " + insert_count, 10, (TEXT_SIZE + 10) * 3);
        text("Tree Nodes: " + node_count, 10, (TEXT_SIZE + 10) * 4);
    }

}