import { createClient } from '@supabase/supabase-js';

const supabase = createClient(
  process.env.SUPABASE_URL,
  process.env.SUPABASE_KEY
);

export default async function handler(req, res) {
  const { data, error } = await supabase
    .from('datasensor')
    .select('*')
    .order('created_at', { ascending: false })
    .limit(1);

  if (error || data.length === 0) {
    return res.status(500).json({ message: 'No data' });
  }

  const last = data[0];
  const delay = (Date.now() - new Date(last.created_at)) / 1000;

  res.json({
    suhu: last.suhu + '°C',
    kelembaban: last.kelembaban + '%',
    status: delay < 10 ? 'Connected' : 'Disconnected',
    timestamp: last.created_at
  });
}
