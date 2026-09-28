# Boundary

The `Boundary` class represents one boundary of a one-dimensional computational domain. It stores the boundary value, boundary-condition type, orientation of the domain line, and the boundary's location on that line.

## Boundary orientation and location

In a one-dimensional model, a boundary is a point, so the point itself has no meaningful geometric orientation. The `Orientation` member is used here to describe whether the line containing the boundary is horizontal or vertical. Together with `BoundaryLocation`, this lets the physical setup identify the endpoint as `Left` or `Right` on a horizontal line, or as `Bottom` or `Top` on a vertical line.

In this project, the one-dimensional domain is represented by a vertical line. Its boundary points are therefore identified as the bottom and top boundaries (`BoundaryLocation::Bottom` and `BoundaryLocation::Top`) with `Orientation::Vertical`.

## Stored values

- `BoundaryValue` is the value associated with the boundary.
- `Type` identifies the boundary condition: `Dirichlet`, `Neumann`, or `Robin`.
- `orientation` records whether the domain line is horizontal or vertical.
- `Location` identifies the endpoint: `Left`, `Right`, `Bottom`, or `Top`.

The constructor takes these four values in this order:

```cpp
Boundary(
    BoundaryType Type_,
    Orientation Orientation_,
    BoundaryLocation Location_,
    double BoundaryValue_
);
```

For example, a fixed-value bottom boundary on this project's vertical line can be created as:

```cpp
Boundary BottomBoundary(
    BoundaryType::Dirichlet,
    Orientation::Vertical,
    BoundaryLocation::Bottom,
    BoundaryValue
);
```

## Accessors and setters

`GetValue()`, `GetType()`, `GetOrientation()`, and `GetLocation()` return the stored properties. `SetValue()`, `SetType()`, and `SetLocation()` update their corresponding properties. Orientation is set at construction and has no setter.

The class stores boundary metadata and a value; it does not itself apply a boundary condition to a field.
