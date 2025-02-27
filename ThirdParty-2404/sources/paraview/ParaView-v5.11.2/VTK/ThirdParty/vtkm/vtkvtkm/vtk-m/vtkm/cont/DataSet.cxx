//============================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//============================================================================

#include <vtkm/cont/DataSet.h>

namespace vtkm
{
namespace cont
{
void DataSet::Clear()
{
  this->CoordSystems.clear();
  this->Fields.clear();
  this->CellSet = this->CellSet.NewInstance();
}

vtkm::Id DataSet::GetNumberOfCells() const
{
  return this->CellSet.GetNumberOfCells();
}

vtkm::Id DataSet::GetNumberOfPoints() const
{
  if (this->CoordSystems.empty())
  {
    return 0;
  }
  return this->CoordSystems[0].GetNumberOfPoints();
}

void DataSet::CopyStructure(const vtkm::cont::DataSet& source)
{
  this->CoordSystems = source.CoordSystems;
  this->CellSet = source.CellSet;
}

const vtkm::cont::Field& DataSet::GetField(vtkm::Id index) const
{
  VTKM_ASSERT((index >= 0) && (index < this->GetNumberOfFields()));
  auto it = this->Fields.cbegin();
  std::advance(it, index);
  return it->second;
}

vtkm::cont::Field& DataSet::GetField(vtkm::Id index)
{
  VTKM_ASSERT((index >= 0) && (index < this->GetNumberOfFields()));
  auto it = this->Fields.begin();
  std::advance(it, index);
  return it->second;
}

vtkm::Id DataSet::GetFieldIndex(const std::string& name, vtkm::cont::Field::Association assoc) const
{
  const auto it = this->Fields.find({ name, assoc });
  if (it != this->Fields.end())
  {
    return static_cast<vtkm::Id>(std::distance(this->Fields.begin(), it));
  }
  return -1;
}

const vtkm::cont::Field& DataSet::GetField(const std::string& name,
                                           vtkm::cont::Field::Association assoc) const
{
  auto idx = this->GetFieldIndex(name, assoc);
  if (idx == -1)
  {
    throw vtkm::cont::ErrorBadValue("No field with requested name: " + name);
  }

  return this->GetField(idx);
}

vtkm::cont::Field& DataSet::GetField(const std::string& name, vtkm::cont::Field::Association assoc)
{
  auto idx = this->GetFieldIndex(name, assoc);
  if (idx == -1)
  {
    throw vtkm::cont::ErrorBadValue("No field with requested name: " + name);
  }

  return this->GetField(idx);
}

const vtkm::cont::CoordinateSystem& DataSet::GetCoordinateSystem(vtkm::Id index) const
{
  VTKM_ASSERT((index >= 0) && (index < this->GetNumberOfCoordinateSystems()));
  return this->CoordSystems[static_cast<std::size_t>(index)];
}

vtkm::cont::CoordinateSystem& DataSet::GetCoordinateSystem(vtkm::Id index)
{
  VTKM_ASSERT((index >= 0) && (index < this->GetNumberOfCoordinateSystems()));
  return this->CoordSystems[static_cast<std::size_t>(index)];
}

vtkm::Id DataSet::GetCoordinateSystemIndex(const std::string& name) const
{
  vtkm::Id index = -1;
  for (auto i = this->CoordSystems.begin(); i != this->CoordSystems.end(); ++i)
  {
    if (i->GetName() == name)
    {
      index = static_cast<vtkm::Id>(std::distance(this->CoordSystems.begin(), i));
      break;
    }
  }
  return index;
}

const vtkm::cont::CoordinateSystem& DataSet::GetCoordinateSystem(const std::string& name) const
{
  vtkm::Id index = this->GetCoordinateSystemIndex(name);
  if (index < 0)
  {
    std::string error_message("No coordinate system with the name " + name +
                              " valid names are: \n");
    for (const auto& cs : this->CoordSystems)
    {
      error_message += cs.GetName() + "\n";
    }
    throw vtkm::cont::ErrorBadValue(error_message);
  }
  return this->GetCoordinateSystem(index);
}

vtkm::cont::CoordinateSystem& DataSet::GetCoordinateSystem(const std::string& name)
{
  vtkm::Id index = this->GetCoordinateSystemIndex(name);
  if (index < 0)
  {
    std::string error_message("No coordinate system with the name " + name +
                              " valid names are: \n");
    for (const auto& cs : this->CoordSystems)
    {
      error_message += cs.GetName() + "\n";
    }
    throw vtkm::cont::ErrorBadValue(error_message);
  }
  return this->GetCoordinateSystem(index);
}

void DataSet::PrintSummary(std::ostream& out) const
{
  out << "DataSet:\n";
  out << "  CoordSystems[" << this->CoordSystems.size() << "]\n";
  for (std::size_t index = 0; index < this->CoordSystems.size(); index++)
  {
    this->CoordSystems[index].PrintSummary(out);
  }

  out << "  CellSet \n";
  this->GetCellSet().PrintSummary(out);

  out << "  Fields[" << this->GetNumberOfFields() << "]\n";
  for (vtkm::Id index = 0; index < this->GetNumberOfFields(); index++)
  {
    this->GetField(index).PrintSummary(out);
  }

  out.flush();
}

VTKM_CONT void DataSet::AddField(const Field& field)
{
  // map::insert will not replace the duplicated element
  this->Fields[{ field.GetName(), field.GetAssociation() }] = field;
}

void DataSet::ConvertToExpected()
{
  for (vtkm::IdComponent coordIndex = 0; coordIndex < this->GetNumberOfCoordinateSystems();
       ++coordIndex)
  {
    this->GetCoordinateSystem(coordIndex).ConvertToExpected();
  }

  for (vtkm::IdComponent fieldIndex = 0; fieldIndex < this->GetNumberOfFields(); ++fieldIndex)
  {
    this->GetField(fieldIndex).ConvertToExpected();
  }
}

} // namespace cont
} // namespace vtkm
