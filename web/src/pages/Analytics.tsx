import React from 'react';
import { getMockPowerHistory } from '../services/mockData';
import { BarChart, Bar, XAxis, YAxis, CartesianGrid, Tooltip, ResponsiveContainer, PieChart, Pie, Cell } from 'recharts';

const Analytics: React.FC = () => {
  const powerHistory = getMockPowerHistory();
  
  // Aggregate by hour
  const hourlyData = Array.from({ length: 24 }, (_, i) => {
    const hour = i;
    const readings = powerHistory.filter(r => r.timestamp.getHours() === hour);
    const avgPower = readings.reduce((sum, r) => sum + r.power, 0) / readings.length;
    return {
      hour: `${hour}:00`,
      power: avgPower || 0,
    };
  });

  const deviceUsage = [
    { name: 'Living Room', value: 145 },
    { name: 'Kitchen', value: 230 },
    { name: 'Office', value: 95 },
    { name: 'Bedroom', value: 75 },
    { name: 'Garage', value: 380 },
  ];

  const COLORS = ['#2563eb', '#10b981', '#f59e0b', '#ef4444', '#8b5cf6'];

  return (
    <div className="page">
      <h1 className="page-title">Analytics</h1>

      <div className="card" style={{ marginBottom: '1.5rem' }}>
        <h2>Hourly Power Consumption</h2>
        <ResponsiveContainer width="100%" height={300}>
          <BarChart data={hourlyData}>
            <CartesianGrid strokeDasharray="3 3" />
            <XAxis dataKey="hour" />
            <YAxis />
            <Tooltip />
            <Bar dataKey="power" fill="#2563eb" />
          </BarChart>
        </ResponsiveContainer>
      </div>

      <div className="card">
        <h2>Energy Usage by Device (kWh)</h2>
        <ResponsiveContainer width="100%" height={300}>
          <PieChart>
            <Pie
              data={deviceUsage}
              cx="50%"
              cy="50%"
              labelLine={false}
              label={(entry) => entry.name}
              outerRadius={80}
              fill="#8884d8"
              dataKey="value"
            >
              {deviceUsage.map((entry, index) => (
                <Cell key={`cell-${index}`} fill={COLORS[index % COLORS.length]} />
              ))}
            </Pie>
            <Tooltip />
          </PieChart>
        </ResponsiveContainer>
      </div>
    </div>
  );
};

export default Analytics;
